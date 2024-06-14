#include <points.h>
#include <string>
#include <unordered_set>
#include <vector>
namespace test {

void generate_circle_test(int num_points);
void generate_double_circle_test(int num_points);
void generate_square_test(int num_points);

void check_test(const std::string &input_test, const std::string &correction,
                std::vector<Point> F(std::vector<Point> &));

void compare_files(const std::string &file1, const std::string &file2);
void generate_tests();
void robust_tests(std::vector<Point> F(std::vector<Point> &));
void run_file(const std::string &file_name);

double time_function(std::vector<Point> (*F)(std::vector<Point> &),
                     std::vector<Point> &points);

} // namespace test
