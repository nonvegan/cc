#include "jpeg.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <jpeglib.h>
#include <stdbool.h>

void jpeg_save_canvas_to_file(Canvas *canvas, const char *file_name, size_t quality)
{
    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        fprintf(stdout, "ERROR: could not open file %s: %s\n", file_name, strerror(errno));
        return;
    }

    struct jpeg_error_mgr jpeg_error_info;
    struct jpeg_compress_struct jpeg_compress_info;

    jpeg_compress_info.err = jpeg_std_error(&jpeg_error_info);
    jpeg_create_compress(&jpeg_compress_info);
    jpeg_stdio_dest(&jpeg_compress_info, f);

    jpeg_compress_info.in_color_space = JCS_RGB;
    jpeg_compress_info.input_components = 3;
    jpeg_compress_info.image_width = canvas->width;
    jpeg_compress_info.image_height = canvas->height;
    jpeg_set_defaults(&jpeg_compress_info);

    if(quality > 100) {
        printf("WARN: JPEG quality %zu outside of [0, 100], falling back\n", quality);
        quality = 100;
    }

    jpeg_set_quality(&jpeg_compress_info, quality, true);
    jpeg_start_compress(&jpeg_compress_info, true);

    JSAMPROW row_pointer;
    while (jpeg_compress_info.next_scanline < jpeg_compress_info.image_height) {
        row_pointer = (JSAMPROW) &canvas->ctx[jpeg_compress_info.next_scanline * 3 * canvas->width];
        jpeg_write_scanlines(&jpeg_compress_info, &row_pointer, 1);
    }
    jpeg_finish_compress(&jpeg_compress_info);

    fclose(f);
    printf("INFO: Generated %s\n", file_name);
}
