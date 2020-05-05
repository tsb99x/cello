find_package(OpenMP REQUIRED)

add_library(grid grid.c)
target_link_libraries(grid OpenMP::OpenMP_C)
target_include_directories(grid PUBLIC include)
