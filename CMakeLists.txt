cmake_minimum_required(VERSION 3.10)
project(cello LANGUAGES C)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(glfw)
add_subdirectory(src)
