add_executable(${PROJECT_NAME} WIN32 main.c)
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL OpenMP::OpenMP_C)
