#ifndef CANVAS_H_
#define CANVAS_H_

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    size_t width;
    size_t height;
    uint8_t *ctx;
} Canvas;

Canvas *canvas_create(size_t w, size_t h);

void canvas_free(Canvas *canvas);

void canvas_fill_px(Canvas *canvas, size_t x, size_t y, uint32_t c);

void canvas_clear_color(Canvas *canvas, uint32_t c);

void canvas_draw_filled_circle(Canvas *canvas, float cx, float cy, float r, uint32_t fg);

void canvas_draw_circle(Canvas *canvas, size_t cx, size_t cy, size_t r, uint32_t fg);

void canvas_save_to_ppm(Canvas *canvas, char* file_name);

#endif // CANVAS_H_
