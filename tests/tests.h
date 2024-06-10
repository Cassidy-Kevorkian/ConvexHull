#include <points.h>
#include <string>
#include <unordered_set>
#include <vector>
namespace test {

void generate_circle_test(int n, const std::string &type);

void check_test(const std::string &input_test, const std::string &correction,
                std::vector<Point> F(std::vector<Point> &));

void compare_files(const std::string &file1, const std::string &file2);
void generate_tests();
void run_tests(std::vector<std::string> types, std::vector<Point> F(std::vector<Point> &));
void run_file(const std::string &file_name);


} // namespace test
