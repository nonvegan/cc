#include "canvas.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <math.h>

Canvas *canvas_create(size_t w, size_t h)
{
    Canvas *canvas = malloc(sizeof(Canvas));
    if(canvas == NULL) exit(1);

    canvas->width = w;
    canvas->height = h;
    canvas->ctx = calloc(w * h * 3, 1);

    return canvas;
}

void canvas_free(Canvas *canvas)
{
    free(canvas->ctx);
    free(canvas);
}

void canvas_exit(Canvas *canvas, int status)
{
    canvas_free(canvas);
    exit(status);
}

void canvas_change_size(Canvas *canvas, size_t w, size_t h)
{
    uint8_t *buffer = calloc(w * h * 3, 1);

    size_t min_width = fmin(w, canvas->width);
    size_t min_height = fmin(h, canvas->height);

    for(size_t row = 0; row < min_height; row++) {
        memcpy(buffer + 3 * row * w, canvas->ctx + 3 * row * canvas->width, 3 * min_width);
    }

    canvas->width = w;
    canvas->height = h;
    free(canvas->ctx);
    canvas->ctx = buffer;
}

void canvas_fill_px(Canvas *canvas, size_t x, size_t y, uint32_t c)
{
    assert(x < canvas->width && y < canvas->height);

    size_t px_offset = (y * canvas->width + x) * 3;
    canvas->ctx[px_offset + 0] = R_RGB(c);
    canvas->ctx[px_offset + 1] = G_RGB(c);
    canvas->ctx[px_offset + 2] = B_RGB(c);
}

uint32_t canvas_get_px(Canvas *canvas, size_t x, size_t y)
{
    assert(x < canvas->width && y < canvas->height);

    size_t px_offset = (y * canvas->width + x) * 3;
    return RGB(canvas->ctx[px_offset + 0],
               canvas->ctx[px_offset + 1],
               canvas->ctx[px_offset + 2]);
}

void canvas_clear(Canvas *canvas, uint32_t c)
{
    for(size_t x = 0; x < canvas->width; x++)
        for(size_t y = 0; y < canvas->height; y++)
            canvas_fill_px(canvas, x, y, c);
}

void canvas_map(Canvas *canvas, uint32_t (fill_color_callback) (size_t _x, size_t _y)) {
    for(size_t x = 0; x < canvas->width; x++)
        for(size_t y = 0; y < canvas->height; y++)
            canvas_fill_px(canvas, x, y, fill_color_callback(x, y));
}

void canvas_draw_circle(Canvas *canvas, float cx, float cy, float r, uint32_t fg)
{
    float x = 0.5f;
    float y = r - 0.5f;

    while(x <= y) {
        canvas_fill_px(canvas, cx + x, cy + y, fg);
        canvas_fill_px(canvas, cx - x, cy + y, fg);
        canvas_fill_px(canvas, cx + x, cy - y, fg);
        canvas_fill_px(canvas, cx - x, cy - y, fg);
        canvas_fill_px(canvas, cy + y, cx + x, fg);
        canvas_fill_px(canvas, cy + y, cx - x, fg);
        canvas_fill_px(canvas, cy - y, cx + x, fg);
        canvas_fill_px(canvas, cy - y, cx - x, fg);

        x++;
        if(x * x + y*y > r * r)
            y--;
    }
}

void canvas_draw_filled_circle(Canvas *canvas, float cx, float cy, float r, uint32_t fg)
{
    for(size_t x = 0; x < canvas->width; x++)
        for(size_t y = 0; y < canvas->height; y++) {
            float dx =  cx - (x + 0.5f);
            float dy =  cy - (y + 0.5f);

            if(dx*dx + dy*dy <= r * r)
                canvas_fill_px(canvas, x, y, fg);
        }
}

float lerp(float x, float y, float p)
{
    return x + p * (y - x);
}

uint32_t color_blend(uint32_t bg, uint32_t fg, float ratio)
{
    return RGB((uint8_t) sqrt(lerp(pow(R_RGB(bg), 2), pow(R_RGB(fg), 2), ratio)),
            (uint8_t) sqrt(lerp(pow(G_RGB(bg), 2), pow(G_RGB(fg), 2), ratio)),
            (uint8_t) sqrt(lerp(pow(B_RGB(bg), 2), pow(B_RGB(fg), 2), ratio)));
}

void canvas_draw_anti_aliased_filled_circle(Canvas *canvas, float cx, float cy, float r, uint32_t fg, uint32_t bg, size_t aa_x)
{
    for(size_t x = 0; x < canvas->width; x++)
        for(size_t y = 0; y < canvas->height; y++) {
            size_t aa_count = 0;
            float aa_step = 1.0f / (1 + (float) aa_x);

            for(size_t i = 1; i <= aa_x; i++)
                for (size_t j = 1; j <= aa_x; j++) {
                    float dx =  cx - (x + i * aa_step);
                    float dy =  cy - (y + j * aa_step);

                    if(dx*dx + dy*dy <= r * r)
                        aa_count++;
                }

            canvas_fill_px(canvas, x, y, color_blend(bg, fg, (float) aa_count / (aa_x * aa_x)));
        }
}

void canvas_save_to_ppm(Canvas *canvas, char* file_name)
{
    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        fprintf(stdout, "ERROR: could not open file %s: %s\n", file_name, strerror(errno));
        canvas_free(canvas);
        exit(1);
    }

    fputs("P6\n", f);
    fprintf(f, "%lu %lu 255\n", canvas->width, canvas->height);
    fwrite(canvas->ctx, canvas->width * canvas->height * 3, 1, f);

    fclose(f);
    printf("INFO: Generated %s\n", file_name);
}
