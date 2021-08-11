#include "./canvas.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

Canvas *canvas_create(size_t w, size_t h)
{
    Canvas *canvas = malloc(sizeof(canvas));

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

void canvas_fill_px(Canvas *canvas, size_t x, size_t y, uint32_t c)
{
    assert(x < canvas->width && y < canvas->height);

    size_t px_offset = (y * canvas->height + x) * 3;
    canvas->ctx[px_offset + 0] = (c >> (8 * 2)) & 0xFF;
    canvas->ctx[px_offset + 1] = (c >> (8 * 1)) & 0xFF;
    canvas->ctx[px_offset + 2] = (c >> (8 * 0)) & 0xFF;
}

void canvas_clear(Canvas *canvas, uint32_t c)
{
    for(size_t x = 0; x < canvas->width; x++)
        for(size_t y = 0; y < canvas->height; y++)
            canvas_fill_px(canvas, x, y, c);
}

void canvas_draw_filled_circle(Canvas *canvas, float cx, float cy, float r, uint32_t fg)
{
    for(size_t x = 0; x < canvas->width; x++) {
        for(size_t y = 0; y < canvas->height; y++) {
            float dx =  cx - (x + 0.5f);
            float dy =  cy - (y + 0.5f);

            if(dx*dx + dy*dy <= r * r)
                canvas_fill_px(canvas, x, y, fg);
        }
    }
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

void canvas_save_to_ppm(Canvas *canvas, char* file_name)
{
    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        fprintf(stdout, "ERROR: could not open file %s: %s\n", file_name, strerror(errno));
        exit(1);
    }

    fputs("P6\n", f);
    fprintf(f, "%lu %lu 255\n", canvas->width, canvas->height);
    fwrite(canvas->ctx, canvas->width * canvas->height * 3, 1, f);

    fclose(f);
}
