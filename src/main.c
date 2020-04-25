#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>
#include <GLFW/glfw3.h>

#define TITLE_LEN 255

static int idx_at(
        int width,
        int x,
        int y
) {
        return y * width + x;
}

static void rand_field(
        char field[],
        int width,
        int height
) {
        int x, y;

        for (y = 0; y < height; ++y)
                for (x = 0; x < width; ++x)
                        field[idx_at(width, x, y)] = rand() % 2;
}

static void clear_border(
        char field[],
        int width,
        int height
) {
        int i;

        for (i = 0; i < width; ++i) {
                field[idx_at(width, i, 0)] = 0;
                field[idx_at(width, i, height - 1)] = 0;
        }
        for (i = 0; i < height; ++i) {
                field[idx_at(width, 0, i)] = 0;
                field[idx_at(width, width - 1, i)] = 0;
        }
}

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

static void do_tick(
        char field[],
        char buffer[],
        int width,
        int height
) {
        int x, y, pos, alive;

        memset(buffer, 0, width * height * sizeof(char));

        #pragma omp parallel for
        for (y = 1; y < height - 1; ++y)
                for (x = 1; x < width - 1; ++x) {
                        pos = y * width + x;
                        alive = field[pos - width - 1]
                                + field[pos - width]
                                + field[pos - width + 1]
                                + field[pos - 1]
                                + field[pos + 1]
                                + field[pos + width - 1]
                                + field[pos + width]
                                + field[pos + width + 1];
                        if (alive == 3)
                                buffer[pos] = 1;
                        else if (alive == 2 && field[pos] == 1)
                                buffer[pos] = 1;
                }
}

static void draw_field(
        char field[],
        int width,
        int height
) {
        int x, y, pos;

        glColor3f(1, 0.33f, 0);
        glBegin(GL_POINTS);
        pos = 0;
        for (y = 0; y < height; ++y)
                for (x = 0; x < width; ++x) {
                        if (field[pos] == 1)
                                glVertex2f(x, y);
                        ++pos;
                }
        glEnd();
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
        char *field, *buffer, *temp, title[TITLE_LEN];
        GLFWwindow *window;

        srand(time(NULL));
        printf("OpenMP threads count: %d\n", omp_get_max_threads());

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

        field = malloc(width * height * sizeof(char));
        if (!field) {
                glfwTerminate();
                return -1;
        }

        buffer = malloc(width * height * sizeof(char));
        if (!buffer) {
                glfwTerminate();
                return -1;
        }

        rand_field(field, width, height);
        clear_border(field, width, height);

        prev = glfwGetTime();
        draw_time = 0;
        iter = 0;
        while (!glfwWindowShouldClose(window)) {
                before_draw = glfwGetTime();
                glClear(GL_COLOR_BUFFER_BIT);
                setup_camera(width, height);
                draw_field(field, width, height);
                draw_time += glfwGetTime() - before_draw;

                do_tick(field, buffer, width, height);

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

        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
}
