#pragma once

void rand_field(
        char field[],
        int width,
        int height
);

void clear_border(
        char field[],
        int width,
        int height
);

void do_tick(
        char field[],
        char buffer[],
        int width,
        int height
);

void draw_field(
        char field[],
        int width,
        int height
);
