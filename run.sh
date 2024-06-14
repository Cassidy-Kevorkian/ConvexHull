[ -d build_directory ] || mkdir build_directory && cd build_directory

cmake .. &> /dev/null
cmake --build . --parallel $(nproc) &> /dev/null

./convex_hull







