cmake_minimum_required(VERSION 3.17)
project(ConvexHull)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")


add_executable(convex_hull main.cpp)

add_library(quick_hull STATIC QuickHull.cpp)
add_library(quick_hull_parallel STATIC QuickHullParallel.cpp)
add_library(points STATIC points.cpp)
add_library(graham_scan STATIC GrahamScan.cpp)
add_library(end_to_end_testing STATIC tests/tests.cpp)
add_library(random_hull STATIC RandomHull.cpp)

target_include_directories(quick_hull PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(quick_hull_parallel  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(points PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(graham_scan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(end_to_end_testing PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/tests" "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(random_hull PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")


target_link_libraries(convex_hull PUBLIC end_to_end_testing quick_hull quick_hull_parallel points graham_scan random_hull) 





