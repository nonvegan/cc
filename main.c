#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#define WIDTH_PX 256
#define HEIGHT_PX 256
#define RADIUS_PX (WIDTH_PX / 3)

uint8_t *ctx_create(size_t w, size_t h)
{
    return malloc(w * h * 3 * sizeof(uint8_t));
}

void ctx_free(uint8_t *ctx)
{
    free(ctx);
}

static void ctx_fill_px(uint8_t *ctx, size_t w, size_t h, size_t x, size_t y, uint32_t c)
{
    size_t px_offset = (y * w + x) * 3;
    ctx[px_offset + 0] = (c >> (8 * 2)) & 0xFF;
    ctx[px_offset + 1] = (c >> (8 * 1)) & 0xFF;
    ctx[px_offset + 2] = (c >> (8 * 0)) & 0xFF;
}

void ctx_clear_color(uint8_t *ctx, size_t w, size_t h, uint32_t c)
{
    for(size_t x = 0; x < w; x++)
        for(size_t y = 0; y < w; y++)
            ctx_fill_px(ctx, w, h, x, y, c);
}

void ctx_draw_filled_circle(uint8_t *ctx, size_t w, size_t h, float cx, float cy, float r, uint32_t fg)
{
    for(size_t x = 0; x < w; x++) {
        for(size_t y = 0; y < h; y++) {
            float dx =  cx - (x + 0.5f);
            float dy =  cy - (y + 0.5f);

            if(dx*dx + dy*dy <= r * r)
                ctx_fill_px(ctx, w, h, x, y, fg);
        }
    }
}

void ctx_draw_circle(uint8_t *ctx, size_t w, size_t h, size_t cx, size_t cy, int r, uint32_t fg)
{
    int x = 0;
    int y = r;

    while(x <= y) {
        ctx_fill_px(ctx, w, h, cx + x, cy + y, fg);
        ctx_fill_px(ctx, w, h, cx - x, cy + y, fg);
        ctx_fill_px(ctx, w, h, cx + x, cy - y, fg);
        ctx_fill_px(ctx, w, h, cx - x, cy - y, fg);
        ctx_fill_px(ctx, w, h, cy + y, cx + x, fg);
        ctx_fill_px(ctx, w, h, cy + y, cx - x, fg);
        ctx_fill_px(ctx, w, h, cy - y, cx + x, fg);
        ctx_fill_px(ctx, w, h, cy - y, cx - x, fg);

        x++;
        if(x * x + y*y > r * r)
            y--;
    }
}

void ctx_save_to_ppm(uint8_t *ctx, size_t w, size_t h, char* file_name)
{
    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        fprintf(stdout, "ERROR: could not open file %s: %s\n", file_name, strerror(errno));
        exit(1);
    }

    fputs("P6\n", f);
    fprintf(f, "%u %u 255\n", w, h);
    fwrite(ctx, 3 * w * h, 1, f);

    fclose(f);
}

int main(void)
{
    uint8_t *px_ctx = ctx_create(WIDTH_PX, HEIGHT_PX);

    ctx_clear_color(px_ctx, WIDTH_PX, HEIGHT_PX, 0x282C34); 
    ctx_draw_filled_circle(px_ctx, WIDTH_PX, HEIGHT_PX, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f, RADIUS_PX, 0xF92672);
    ctx_save_to_ppm(px_ctx, WIDTH_PX, HEIGHT_PX, "filled_circle.ppm");

    ctx_clear_color(px_ctx, WIDTH_PX, HEIGHT_PX, 0x282C34); 
    ctx_draw_circle(px_ctx, WIDTH_PX, HEIGHT_PX, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f, RADIUS_PX, 0xF92672);
    ctx_save_to_ppm(px_ctx, WIDTH_PX, HEIGHT_PX, "circle.ppm");

    ctx_free(px_ctx);
    return 0;
}
