#pragma once

struct grid {
        char *field;
        int width;
        int height;
};

struct grid *grid_init(
        int width,
        int height
);

void grid_destroy(
        struct grid *g
);

void grid_rand(
        struct grid *g
);

void grid_clear_border(
        struct grid *g
);

void grid_do_tick(
        struct grid *g,
        struct grid *b
);

void grid_draw_field(
        struct grid *g
);
