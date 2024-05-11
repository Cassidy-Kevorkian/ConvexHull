#include <iostream>
#include <vector>
#include "points.cpp"
#include <thread>

Point FurthestPointFromLineParallel(Line l, std::vector<Point> points, size_t num_points) {
    Point furthest_point = points[0];

    for(int i = 1; i < num_points; ++i) {

        if(dist(furthest_point, l) < dist(points[i], l)) furthest_point = points[i];

   }
    
    return furthest_point;
}


std::vector<std::vector<Point>> GeneratePartitionParallel(Point p, Point q, std::vector<Point> points, size_t num_points) {
    std::vector<std::vector<Point>> partition; 
    std::vector<Point> part_1, part_2;
    
    for(int i = 0; i < num_points; ++i) {

        if(((p.x == points[i].x) && (p.y == points[i].y)) || ((q.x == points[i].x) && (q.y == points[i].y))) continue;

        if(cross_prod(p, q, p, points[i]) >= 0) part_1.push_back(points[i]);
    
        else part_2.push_back(points[i]);

    }

    return {part_1, part_2};
}


std::vector<Point> GeneratePointsOutsideParallel(Point p, Point q, Point furthest_point, std::vector<Point> points, size_t num_points) {
    std::vector<Point> points_outside;
    
    for(int i = 0; i < num_points; ++i) {
        
        if((furthest_point.x == points[i].x) && (furthest_point.y == points[i].y)) continue;

        if(angle(p, q, p, furthest_point) <= angle(p, q, p, points[i])) points_outside.push_back(points[i]);

    }
    
    return points_outside;
}


void QuickHullParallelRec(Point p, Point q, std::vector<Point> points, std::vector<Point> &convex_hull) {
    size_t num_points = points.size();
    
    if(num_points == 0) return; 

    Line l = generate_line(p, q);
    Point furthest_point = FurthestPointFromLineParallel(l, points, num_points);
    convex_hull.push_back(furthest_point);

    std::vector<Point> part_1 = GeneratePointsOutsideParallel(p, q, furthest_point, points, num_points);
    std::vector<Point> part_2 = GeneratePointsOutsideParallel(q, p, furthest_point, points, num_points);
	
    std::thread t1 = std::thread(&QuickHullParallelRec, std::ref(p), std::ref(furthest_point), std::ref(part_1), std::ref(convex_hull)); 
    std::thread t2 = std::thread(&QuickHullParallelRec, std::ref(q), std::ref(furthest_point), std::ref(part_2), std::ref(convex_hull)); 
    
    t1.join();
    t2.join();
} 


std::vector<Point> QuickHullParallel(std::vector<Point> points) {
    size_t num_points = points.size();
    Point highest_point = points[0], lowest_point = points[0];
    
    for(int i = 0; i < num_points; ++i) {
        
        if(highest_point.y < points[i].y) highest_point = points[i];
    
        if(lowest_point.y > points[i].y) lowest_point = points[i];
 
    }

    std::vector<std::vector<Point>> initial_partition = GeneratePartitionParallel(lowest_point, highest_point, points, num_points);
    std::vector<Point> part_1 = initial_partition[0], part_2 = initial_partition[1];
    std::vector<Point> convex_hull; 
    convex_hull.push_back(lowest_point);
    convex_hull.push_back(highest_point);
	
    std::thread t1 = std::thread(&QuickHullParallelRec, std::ref(lowest_point), std::ref(highest_point), std::ref(part_1), std::ref(convex_hull));
    std::thread t2 = std::thread(&QuickHullParallelRec, std::ref(lowest_point), std::ref(highest_point), std::ref(part_2), std::ref(convex_hull));
    
    t1.join(); 
    t2.join();
    
    return convex_hull;
}
