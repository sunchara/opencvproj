cmake_minimum_required(VERSION 2.8)

project( INIH_LIB )


add_library(inih STATIC ini.c ./cpp/INIReader.cpp)
add_library(inih_shared SHARED ini.c ./cpp/INIReader.cpp)

target_include_directories(inih PUBLIC ./cpp)
target_include_directories(inih_shared PUBLIC ./cpp)
