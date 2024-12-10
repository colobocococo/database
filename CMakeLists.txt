cmake_minimum_required(VERSION 3.24)
project(c++)
add_library(my_tables.h, database.cpp, parser.cpp)
add_executable(main.cpp)
