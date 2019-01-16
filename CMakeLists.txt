cmake_minimum_required(VERSION 3.12)
project(Matrix2)

set(CMAKE_CXX_STANDARD 17)

add_executable(Matrix2 main.cpp include/utility.h include/matrix.h include/index.h include/iterator.h include/builder.h include/tools.h)