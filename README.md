# Convex Hull Sequential and Parallel

**Overview:**
The Convex Hull Sequential and Parallel project provides implementations of various algorithms for computing the convex hull of a set of points sequentially and in parallel. The convex hull is a fundamental geometric concept widely used in computational geometry and related fields. This project explores different sequential and parallel implementations, aiming to optimize and compare their performance.

**Features:**
- **Algorithms:** Includes implementations of several convex hull algorithms such as QuickHull, Graham Scan, and Randomized Hull.
- **Parallelization:** Utilizes parallel computing techniques to accelerate convex hull computation on multi-core processors.
- **Testing Suite:** Provides a comprehensive testing suite to validate the correctness and performance of the algorithms across different scenarios.

**Usage:**
1. **Building the Project:**
   - Clone the repository and navigate to the project directory.
   - Execute the `run.sh` script to build the project and set up the environment.

2. **Generating Test Cases:**
   - Open `main.cpp` and uncomment the `test::generate()` line to generate test cases and their expected outputs.
   - Run `run.sh` again to generate the test files in `tests/test_files` directory.

3. **Running Algorithms:**
   - Comment out the test generation line (`test::generate()`) in `main.cpp`.
   - Uncomment the line corresponding to the desired algorithm implementation (e.g., `test::robust_tests(quick_hull::convex_hull_parallel);` for parallel QuickHull).
   - Execute `run.sh` to run the algorithm, which computes the average execution time over multiple runs and excludes outliers.
