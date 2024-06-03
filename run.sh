[ -d build_directory ] || mkdir build_directory && cd build_directory

cmake ..
cmake --build . --parallel $(nproc)

./convex_hull







