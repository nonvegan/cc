#ifndef PNG_H_
#define PNG_H_

#include "canvas.h"

#include <stdlib.h>

void png_load_image_to_canvas(const char *src_file_name, Canvas *canvas, int pos_x, int pos_y);

#endif // PNG_H_
