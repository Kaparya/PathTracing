cmake_minimum_required(VERSION 3.8)

project(WorkingProject)
set(CMAKE_CXX_STANDARD 17)

add_executable(WorkingProject "src/main.cpp"
        src/Random/Sobol.cpp)
target_include_directories(WorkingProject PUBLIC src/externals/ src/externals/glm/)
