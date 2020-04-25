cmake_minimum_required(VERSION 3.10)
project(cello LANGUAGES C)

find_package(OpenMP REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(glfw)
add_subdirectory(src)
