find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} grid glfw_w)

add_library(grid grid.cpp grid.hpp)
target_link_libraries(grid PRIVATE glfw_w OpenMP::OpenMP_CXX)

add_library(glfw_w glfw_w.cpp glfw_w.hpp)
target_link_libraries(glfw_w PRIVATE glfw OpenGL::GL)
