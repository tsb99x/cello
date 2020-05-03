find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} WIN32 main.c)
target_link_libraries(${PROJECT_NAME} grid glfw OpenGL::GL)
