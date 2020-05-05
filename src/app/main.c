#include <grid.h>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <excpt.h>

#include <GLFW/glfw3.h>

#define TITLE_LEN 255

static void setup_camera(
        int width,
        int height
) {
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, height, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

static void key_callback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods
) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int WINAPI WinMain (
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        PSTR lpCmdLine,
        INT nCmdShow
) {
        int width, height, iter;
        double prev, before_draw, draw_time;
        char title[TITLE_LEN];
        struct grid *field, *buffer, *temp;
        GLFWwindow *window;

        #ifdef WIN32
        __try {
        #endif

        srand(time(NULL));

        if (!glfwInit())
                return -1;

        width = 1920;
        height = 1080;
        window = glfwCreateWindow(width, height, "Cello", NULL, NULL);
        if (!window) {
                glfwTerminate();
                return -1;
        }

        glfwSetKeyCallback(window, key_callback);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);

        field = grid_init(width, height);
        if (!field) {
                glfwTerminate();
                return -1;
        }

        buffer = grid_init(width, height);
        if (!buffer) {
                glfwTerminate();
                return -1;
        }

        grid_rand(field);
        grid_clear_border(field);

        prev = glfwGetTime();
        draw_time = 0;
        iter = 0;
        while (!glfwWindowShouldClose(window)) {
                before_draw = glfwGetTime();
                glClear(GL_COLOR_BUFFER_BIT);
                setup_camera(width, height);
                grid_draw_field(field);
                draw_time += glfwGetTime() - before_draw;

                grid_do_tick(field, buffer);

                temp = field;
                field = buffer;
                buffer = temp;

                glfwSwapBuffers(window);
                glfwPollEvents();

                if (++iter >= 100) {
                        sprintf(title, "Cello iter: 100@%.3f draw: %.3f", glfwGetTime() - prev, draw_time);
                        glfwSetWindowTitle(window, title);
                        draw_time = 0;
                        prev = glfwGetTime();
                        iter = 0;
                }
        }

        grid_destroy(field);
        grid_destroy(buffer);

        glfwDestroyWindow(window);
        glfwTerminate();

        #ifdef WIN32
        } __except(EXCEPTION_EXECUTE_HANDLER) {
                fprintf(stderr,
                        "Exception occurred, code: %d\n",
                        GetExceptionCode());
                return -1;
        }
        #endif

        return 0;
}
