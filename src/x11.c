#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "x11.h"

Canvas *x11_screenshot_as_canvas(void)
{
    Display *display = XOpenDisplay(NULL);
    if(display == NULL) {
        fprintf(stderr, "ERROR: Could not connect to the X Server\n");
        exit(1);
    }
    printf("INFO: Connected to the X Server\n");

    Window window = DefaultRootWindow(display);

    XWindowAttributes wa;
    XGetWindowAttributes(display, window, &wa);

    XImage *image = XGetImage(display, window, 0, 0, wa.width, wa.height, AllPlanes, ZPixmap);
    if(image == NULL) {
        fprintf(stderr, "ERROR: Could not get X Image\n");
        exit(1);
    }
    printf("INFO: Got X Image %dx%d/%d\n", image->width, image->height, image->bits_per_pixel);

    Canvas *canvas = canvas_create(image->width, image->height);

    //TODO: Directly copying pixel data from image->data to canvas->ctx instead of calling canvas_fill_px and XGetPixel would be nice
    for(size_t x = 0; x < image->width; x++)
        for(size_t y = 0; y < image->height; y++)
            canvas_fill_px(canvas, x, y, XGetPixel(image, x, y));

    XDestroyImage(image);
    XCloseDisplay(display);
    return canvas;
}
