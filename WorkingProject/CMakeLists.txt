cmake_minimum_required(VERSION 3.8)

project(WorkingProject)
set(CMAKE_CXX_STANDARD 17)

add_executable(WorkingProject "main.cpp"
        Random/Sobol.cpp)
target_include_directories(WorkingProject PUBLIC externals/ externals/glm/)
