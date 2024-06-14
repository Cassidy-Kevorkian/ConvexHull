# ConvexHull

Overview:
The Convex Hull project provides implementations of various algorithms for computing the convex hull of a set of points. The convex hull is a fundamental geometric concept widely used in computational geometry and related fields.

Features:

Algorithms: Includes implementations of several convex hull algorithms such as QuickHull, Graham Scan, and Randomized Hull.
Parallelization: Utilizes parallel computing techniques to accelerate convex hull computation on multi-core processors.
Testing Suite: Provides a comprehensive testing suite to validate the correctness and performance of the algorithms across different scenarios.
Documentation: Detailed documentation to assist users in understanding the project structure, algorithms employed, and how to utilize the codebase effectively.
Usage:

Building the Project:

Clone the repository and navigate to the project directory.
Execute the \texttt{run.sh} script to build the project and set up the environment.
Generating Test Cases:

Open \texttt{main.cpp} and uncomment the \texttt{test::generate()} line to generate test cases and their expected outputs.
Run \texttt{run.sh} again to generate the test files in \texttt{tests/test_files} directory.
Running Algorithms:

Comment out the test generation line (\texttt{test::generate()}) in \texttt{main.cpp}.
Uncomment the line corresponding to the desired algorithm implementation (e.g., \texttt{quick_hull::convex_hull_parallel} for parallel QuickHull).
Execute \texttt{run.sh} to run the algorithm, which computes the average execution time over multiple runs and excludes outliers.
