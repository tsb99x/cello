#include <grid.h>

#include <omp.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>

static int idx_at(
        int width,
        int x,
        int y
) {
        return y * width + x;
}

void rand_field(
        char field[],
        int width,
        int height
) {
        int x, y;

        for (y = 0; y < height; ++y)
                for (x = 0; x < width; ++x)
                        field[idx_at(width, x, y)] = rand() % 2;
}

void clear_border(
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

void do_tick(
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

void draw_field(
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
