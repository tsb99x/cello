find_package(OpenMP REQUIRED)
find_package(OpenGL REQUIRED)

add_library(grid grid.c)
target_link_libraries(grid glfw OpenMP::OpenMP_C OpenGL::GL)
target_include_directories(grid PUBLIC include)
