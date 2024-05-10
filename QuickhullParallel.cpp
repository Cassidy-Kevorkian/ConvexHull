#include <iostream>
#include <vector>
#include "points.cpp"
#include <thread>

Point FurthestPointFromLine(Line l, std::vector<Point> points, size_t num_points) {
    Point furthest_point = points[0];

    for(int i = 1; i < num_points; ++i) {

        if(dist(furthest_point, l) < dist(points[i], l)) furthest_point = points[i];

    }
    
    return furthest_point;
}


std::vector<std::vector<Point>> GeneratePartition(Point p, Point q, std::vector<Point> points, size_t num_points) {
    std::vector<std::vector<Point>> partition; 
    std::vector<Point> part_1, part_2;
    
    for(int i = 0; i < num_points; ++i) {

        if(((p.x == points[i].x) && (p.y == points[i].y)) || ((q.x == points[i].x) && (q.y == points[i].y))) continue;

        if(cross_prod(p, q, p, points[i]) >= 0) part_1.push_back(points[i]);
    
        else part_2.push_back(points[i]);

    }
    
    return {part_1, part_2};
}


std::vector<Point> GeneratePointsOutside(Point p, Point q, Point furthest_point, std::vector<Point> points, size_t num_points) {
    std::vector<Point> answer;
    
    for(int i = 0; i < num_points; ++i) {
        
        if((furthest_point.x == points[i].x) && (furthest_point.y == points[i].y)) continue;

        if(angle(p, q, p, furthest_point) <= angle(p, q, p, points[i])) answer.push_back(points[i]);

    }
    
    return answer;
}


void QuickHullRec(Point &p, Point &q, std::vector<Point> &points, std::vector<Point> &answer) {
    size_t num_points = points.size();
    
    if(num_points == 0) return; 

    Line l = generate_line(p, q);
    Point furthest_point = FurthestPointFromLine(l, points, num_points);
    answer.push_back(furthest_point);

    std::vector<Point> points_1 = GeneratePointsOutside(p, q, furthest_point, points, num_points);
    std::vector<Point> points_2 = GeneratePointsOutside(q, p, furthest_point, points, num_points);
    
    std::thread t1 = std::thread(&QuickHullRec, std::ref(p), std::ref(furthest_point), std::ref(points_1), std::ref(answer)); 
    std::thread t2 = std::thread(&QuickHullRec, std::ref(q), std::ref(furthest_point), std::ref(points_2), std::ref(answer)); 
    
    t1.join();
    t2.join();
}


std::vector<Point> QuickHull(std::vector<Point> points) {
    size_t num_points = points.size();
    Point highest_point = points[0], lowest_point = points[0];
    
    for(int i = 1; i < num_points; ++i) {
        
        if(highest_point.y < points[i].y) highest_point = points[i];
    
        if(highest_point.y == points[i].y && highest_point.x < points[i].y) highest_point = points[i];
    
        if(lowest_point.y > points[i].y) lowest_point = points[i];
 
        if(lowest_point.y == points[i].y && lowest_point.x > points[i].y) lowest_point = points[i];

    }

    std::vector<std::vector<Point>> initial_partition = GeneratePartition(lowest_point, highest_point, points, num_points);
    std::vector<Point> points_1 = initial_partition[0], points_2 = initial_partition[1];
    std::vector<Point> answer; 

    answer.push_back(lowest_point);
    answer.push_back(highest_point);

    std::thread t1 = std::thread(&QuickHullRec, std::ref(lowest_point), std::ref(highest_point), std::ref(points_1), std::ref(answer));
    std::thread t2 = std::thread(&QuickHullRec, std::ref(lowest_point), std::ref(highest_point), std::ref(points_2), std::ref(answer));
    
    t1.join(); 
    t2.join();

    return answer;
}


int main() {
    std::cout << "Please enter the number of points:" << std::endl;
    size_t num_points;
    std::cin >> num_points;
    std::cout << "Enter the points one by one in the  format x y:" << std::endl;
    std::vector<Point> points; 
    
    for(int i = 0; i < num_points; ++i) {
        double x, y;
        std::cin >> x;
        std::cin >> y;
        points.push_back(Point(x, y));
    }
    
    std::vector<Point> answer = QuickHull(points);
    
    std::cout << std::endl << "The points belonging to the convex hull are:" << std::endl;
    
    for(int i = 0; i < answer.size(); ++i) {
        std::cout << answer[i] << std::endl;     
    }

    return 0;
}


