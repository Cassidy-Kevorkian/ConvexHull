#include <points.h>
#include <string>
#include <unordered_set>
#include <vector>
namespace test {

void generate_tests_helper(int n, const std::string &type);

void check_test(const std::string &input_test, const std::string &correction,
                std::vector<Point> F(std::vector<Point> &));

void compare_files(const std::string &file1, const std::string &file2);
void generate_tests(int n);
void run_tests();
void run_file(const std::string &file_name);

} // namespace test
