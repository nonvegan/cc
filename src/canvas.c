#include "./canvas.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define R_RGB(X) (X >> 8 * 2 & 0xFF)
#define G_RGB(X) (X >> 8 * 1 & 0xFF)
#define B_RGB(X) (X >> 8 * 0 & 0xFF)

#define AA_X 2
#define AA_STEP 1.0 / (1 + AA_X)

Canvas *canvas_create(size_t w, size_t h)
{
    Canvas *canvas = malloc(sizeof(Canvas));

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
    canvas->ctx[px_offset + 0] = R_RGB(c);
    canvas->ctx[px_offset + 1] = G_RGB(c);
    canvas->ctx[px_offset + 2] = B_RGB(c);
}

void canvas_clear(Canvas *canvas, uint32_t c)
{
    for(size_t x = 0; x < canvas->width; x++)
        for(size_t y = 0; y < canvas->height; y++)
            canvas_fill_px(canvas, x, y, c);
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
    return (uint8_t) lerp(R_RGB(bg), R_RGB(fg), ratio) << 8 * 2 |
           (uint8_t) lerp(G_RGB(bg), G_RGB(fg), ratio) << 8 * 1 |
           (uint8_t) lerp(B_RGB(bg), B_RGB(fg), ratio) << 8 * 0;
}

void canvas_draw_anti_aliased_filled_circle(Canvas *canvas, float cx, float cy, float r, uint32_t fg, uint32_t bg)
{
    for(size_t x = 0; x < canvas->width; x++)
        for(size_t y = 0; y < canvas->height; y++) {

            size_t aa_count = 0;
            for(size_t i = 1; i <= AA_X; i++)
                for (size_t j = 1; j <= AA_X; j++) {
                    float dx =  cx - (x + i * AA_STEP);
                    float dy =  cy - (y + j * AA_STEP);

                    if(dx*dx + dy*dy <= r * r)
                        aa_count++;
                }

            canvas_fill_px(canvas, x, y, color_blend(bg, fg, (float) aa_count / (AA_X * AA_X)));
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
