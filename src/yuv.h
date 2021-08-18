#ifndef YUV_H_
#define YUV_H_

#include "./canvas.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char *file_name;
    FILE *file;
    size_t width; 
    size_t height;
    size_t fps;
} Y4m2;

Y4m2 *y4m2_open_video(char *file_name, size_t width, size_t height, size_t fps);

void y4m2_dump_canvas_frame(Y4m2 *y4m2, Canvas *canvas, size_t frame_count);

void y4m2_close_video(Y4m2 *y4m2);

#endif // YUV_H_
