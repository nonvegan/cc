#include "png.h"

#include "canvas.h"

#include <stdio.h>
#include <assert.h>
#include <png.h>
#include <string.h>
#include <stdint.h>

void png_load_image_to_canvas(const char *src_file_name, Canvas *canvas, int pos_x, int pos_y)
{
    png_image image = {.version = PNG_IMAGE_VERSION};
    png_image_begin_read_from_file(&image, "assets/KEKW.png");

    printf("INFO: Loaded %s PNG file, %dx%d with %d format\n",
            src_file_name,
            image.width,
            image.height,
            image.format);

    image.format = PNG_FORMAT_RGBA;
    uint8_t *image_buffer = calloc(4 * image.width * image.height, 1);
    png_image_finish_read(&image, NULL, image_buffer, 0, NULL);

    int image_width = image.width;
    int image_height = image.height;
    int canvas_width = canvas->width; 
    int canvas_height = canvas->height; 

    for(int x = 0; x < image_width && pos_x + x < canvas_width; x++) {
        if(pos_x + x < 0) continue;
        for(int y = 0; y < image_height && pos_y + y < canvas_height; y++) {
            if(pos_y + y < 0) continue;
            size_t image_px_offset = 4 * (y * image_width + x);
            if(image_buffer[image_px_offset + 3]) {
                size_t canvas_px_offset = 3 * ((pos_y + y) * canvas_width + pos_x + x);
                memcpy(canvas->ctx + canvas_px_offset, image_buffer + image_px_offset, 3);
            }
        }
    }
    png_image_free(&image);

}
