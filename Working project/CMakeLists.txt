cmake_minimum_required(VERSION 3.8)

project(render)
set(CMAKE_CXX_STANDARD 17)

add_executable(render "main.cpp")
target_include_directories(render PUBLIC externals/ externals/glm/)
