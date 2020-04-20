#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GLFW/glfw3.h>

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
        int x, y, alive;

        memset(buffer, 0, width * height * sizeof(char));

        for (y = 1; y < height - 1; ++y)
                for (x = 1; x < width - 1; ++x) {
                        alive = field[idx_at(width, x - 1, y - 1)]
                                + field[idx_at(width, x, y - 1)]
                                + field[idx_at(width, x + 1, y - 1)]
                                + field[idx_at(width, x - 1, y)]
                                + field[idx_at(width, x + 1, y)]
                                + field[idx_at(width, x - 1, y + 1)]
                                + field[idx_at(width, x, y + 1)]
                                + field[idx_at(width, x + 1, y + 1)];

                        if (alive == 3)
                                buffer[idx_at(width, x, y)] = 1;
                        else if (alive == 2 && field[idx_at(width, x, y)] == 1)
                                buffer[idx_at(width, x, y)] = 1;
                }
}

static void draw_field(
        char field[],
        int width,
        int height
) {
        int x, y;

        glColor3f(1, 0.33f, 0);
        glBegin(GL_POINTS);
        for (y = 0; y < height; ++y)
                for (x = 0; x < width; ++x)
                        if (field[y * width + x] == 1)
                                glVertex2f(x, y);
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
        srand(time(NULL));

        int width, height;
        double now, prev;
        char *field, *buffer, *temp;
        GLFWwindow *window;

        if (!glfwInit())
                return -1;

        width = 640;
        height = 480;
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
        while (!glfwWindowShouldClose(window)) {
                glClear(GL_COLOR_BUFFER_BIT);

                setup_camera(width, height);

                draw_field(field, width, height);
                do_tick(field, buffer, width, height);

                temp = field;
                field = buffer;
                buffer = temp;

                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
}
