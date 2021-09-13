#ifndef CANVAS_H_
#define CANVAS_H_

#include <stdlib.h>
#include <stdint.h>

#define RGB(R, G, B) (R << 8 * 2 | \
                      G << 8 * 1 | \
                      B << 8 * 0)

#define R_RGB(X) (X >> 8 * 2 & 0xFF)
#define G_RGB(X) (X >> 8 * 1 & 0xFF)
#define B_RGB(X) (X >> 8 * 0 & 0xFF)

typedef struct {
    size_t width;
    size_t height;
    uint8_t *ctx;
} Canvas;

Canvas *canvas_create(size_t w, size_t h);

void canvas_free(Canvas *canvas);

void canvas_exit(Canvas *canvas, int status);

void canvas_change_size(Canvas *canvas, size_t w, size_t h);

void canvas_fill_px(Canvas *canvas, size_t x, size_t y, uint32_t c);

void canvas_clear(Canvas *canvas, uint32_t c);

void canvas_draw_circle(Canvas *canvas, float cx, float cy, float r, uint32_t fg);

void canvas_draw_filled_circle(Canvas *canvas, float cx, float cy, float r, uint32_t fg);

void canvas_draw_anti_aliased_filled_circle(Canvas *canvas, float cx, float cy, float r, uint32_t fg, uint32_t bg, size_t aa_x);

void canvas_save_to_ppm(Canvas *canvas, char* file_name);

#endif // CANVAS_H_
