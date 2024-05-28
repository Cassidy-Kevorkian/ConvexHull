
void run_file(const std::string& file_name){
    std::ifstream file(file_name);
    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    size_t num_points;
    file >> num_points;
    std::vector<Point> points;
    for (size_t i = 0; i < num_points; ++i)
    {
        double x, y;
        file >> x >> y;
        points.push_back(Point(x, y));
    }
    file.close();
    // std::vector<Point> convex_hull_graham_scan = GrahamScan(points);
    // std::cout << std::endl << "GrahamScan convex hull:" << std::endl;
    // for(Point point : convex_hull_graham_scan) {
    //     std::cout << point << std::endl;     
    // }
    // std::vector<Point> convex_hull_quick_hull = QuickHull(points);
    // std::cout << std::endl << "QuickHull convex hull:" << std::endl;
    // for(Point point : convex_hull_quick_hull) {
    //     std::cout << point << std::endl;     
    // }
    std::vector<Point> convex_hull_quick_hull_parallel = QuickHullParallel(points);
    std::cout << std::endl << "QuickHullParallel convex hull:" << std::endl;
    for(Point point : convex_hull_quick_hull_parallel) {
        std::cout << point << std::endl;     
    }
}

void run_tests(){
    run_file("../tests/test_few.txt");
    run_file("../tests/test_average.txt");
    run_file("../tests/test_many.txt");
}
