#include "y4m2.h"

#include <errno.h>
#include <string.h>
#include <assert.h>

#include "canvas.h"

#define YCBCR(Y,CB,CR) RGB(Y,CB,CR)

#define Y_YCBCR(Y)   R_RGB(Y)
#define CB_YCBCR(CB) G_RGB(CB)
#define CR_YCBCR(CR) B_RGB(CR)

Y4m2 *y4m2_open_video(const char *file_name, size_t width, size_t height, size_t fps)
{
    Y4m2 *y4m2 = malloc(sizeof(Y4m2));

    FILE *f = fopen(file_name, "w");
    if(f == NULL) {
        return NULL;
    }

    fprintf(f, "YUV4MPEG2 W%lu H%lu F%lu:1 Ip A1:1 C444\n", width, height, fps);

    y4m2->file = f;
    y4m2->file_name = strdup(file_name);
    y4m2->width = width;
    y4m2->height = height;
    y4m2->fps = fps;

    return y4m2;
}

uint32_t rgb_to_ycbcr(uint8_t r, uint8_t g, uint8_t b)
{
    float rf = r, gf = g, bf = b;
    uint8_t y  = 16  + (65.738f   * rf + 129.057f * gf + 25.064f  * bf) / 256.0f;
    uint8_t cb = 128 - (37.945f   * rf + 74.494f  * gf - 112.439f * bf) / 256.0f;
    uint8_t cr = 128 - (-112.439f * rf + 94.154f  * gf + 18.285f  * bf) / 256.0f;

    return YCBCR(y, cb, cr);
}

void y4m2_dump_canvas_frame(Y4m2 *y4m2, Canvas *canvas, size_t frame_count)
{
    assert(y4m2->width == canvas->width && y4m2->height == canvas->height);

    size_t px_count = y4m2->width * y4m2->height;
    uint8_t ycbcr_frame[px_count * 3];

    for(size_t i = 0; i < px_count; i++) {
        uint32_t ycbcr = rgb_to_ycbcr(canvas->ctx[i * 3 + 0],
                                      canvas->ctx[i * 3 + 1],
                                      canvas->ctx[i * 3 + 2]);

        ycbcr_frame[px_count * 0 + i] = (uint8_t) Y_YCBCR(ycbcr);
        ycbcr_frame[px_count * 1 + i] = (uint8_t) CB_YCBCR(ycbcr);
        ycbcr_frame[px_count * 2 + i] = (uint8_t) CR_YCBCR(ycbcr);
    }

    for(size_t i = 0; i < frame_count; i++) {
        fputs("FRAME\n", y4m2->file);
        fwrite(ycbcr_frame, px_count * 3, 1, y4m2->file);
    }
}

void y4m2_close_video(Y4m2 *y4m2)
{
    fclose(y4m2->file);
    printf("INFO: Generated %s                 \n", y4m2->file_name);
    free(y4m2->file_name);
    free(y4m2);
}
