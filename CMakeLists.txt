cmake_minimum_required(VERSION 3.23)
project(OOP_2)

set(CMAKE_CXX_STANDARD 14)

#overwrite this when you want to run another file, i.e add_executable(OOP_2 OOP-2/main.cpp)
add_executable(OOP_2 OOP-1/main.cpp OOP-3/main.cpp)