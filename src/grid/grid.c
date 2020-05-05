#include <grid.h>

#include <stdlib.h>

static int idx_at(
        int width,
        int x,
        int y
) {
        return y * width + x;
}

struct grid *grid_init(
        int width,
        int height
) {
        struct grid *grid;

        grid = (struct grid *) malloc(sizeof(struct grid));
        if (!grid)
                return NULL;
        grid->field = malloc(width * height * sizeof(char));
        if (!grid->field)
                return NULL;
        grid->width = width;
        grid->height = height;
        return grid;
}

void grid_destroy(
        struct grid *g
) {
        free(g->field);
        free(g);
}

void grid_rand(
        struct grid *g
) {
        int i;

        for (i = 0; i < g->width * g->height; ++i)
                g->field[i] = rand() % 2;
}

void grid_clear_border(
        struct grid *g
) {
        int i;

        for (i = 0; i < g->width; ++i) {
                g->field[idx_at(g->width, i, 0)] = 0;
                g->field[idx_at(g->width, i, g->height - 1)] = 0;
        }
        for (i = 0; i < g->height; ++i) {
                g->field[idx_at(g->width, 0, i)] = 0;
                g->field[idx_at(g->width, g->width - 1, i)] = 0;
        }
}

void grid_do_tick(
        struct grid *g,
        struct grid *b
) {
        int x, y, pos, alive;

        memset(b->field, 0, b->width * b->height * sizeof(char));

        #pragma omp parallel for
        for (y = 1; y < g->height - 1; ++y)
                for (x = 1; x < g->width - 1; ++x) {
                        pos = y * g->width + x;
                        alive = g->field[pos - g->width - 1]
                                + g->field[pos - g->width]
                                + g->field[pos - g->width + 1]
                                + g->field[pos - 1]
                                + g->field[pos + 1]
                                + g->field[pos + g->width - 1]
                                + g->field[pos + g->width]
                                + g->field[pos + g->width + 1];
                        if (alive == 3)
                                b->field[pos] = 1;
                        else if (alive == 2 && g->field[pos] == 1)
                                b->field[pos] = 1;
                }
}
