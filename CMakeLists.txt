cmake_minimum_required(VERSION 3.15...3.19)
project(mctslib LANGUAGES CXX)


set (CMAKE_CXX_STANDARD 17)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
add_subdirectory(lib/pybind11)
set(SOURCE_DIR "src/mctslib")
include_directories(${SOURCE_DIR})

pybind11_add_module(mctslibcpp src/mctslib/module.cpp)
