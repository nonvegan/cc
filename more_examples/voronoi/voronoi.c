#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../src/canvas.h"
#include "../../src/vec.h"
#include "../../src/jpeg.h"

#define WIDTH_PX 640
#define HEIGHT_PX 480
#define NUMBER_OF_POINTS 20

typedef struct {
    Vec2i pos;
    uint32_t color;
} Point;


void main()
{
    srand(time(NULL));

    Point points[NUMBER_OF_POINTS];
    Canvas *canvas = canvas_create(WIDTH_PX, HEIGHT_PX);

    for(size_t i = 0; i < NUMBER_OF_POINTS; i++) {
        points[i].pos = vec2i(rand() % WIDTH_PX, rand() % HEIGHT_PX);
        points[i].color = RGB(rand() % 255, rand() % 255, rand() % 255);
    }

    for(size_t x = 0; x < WIDTH_PX; x++)
        for(size_t y = 0; y < HEIGHT_PX; y++) {
            Point *closest_point = points;
            for(size_t i = 1; i < NUMBER_OF_POINTS; i++)
                if(vec2i_dst(vec2i(x,y), closest_point->pos) > vec2i_dst(vec2i(x,y), points[i].pos))
                    closest_point = points + i;
            canvas_fill_px(canvas, x, y, closest_point->color);
        }

    for(size_t i = 0; i < NUMBER_OF_POINTS; i++) {
        canvas_draw_filled_circle(canvas, points[i].pos.x, points[i].pos.y, 2, RGB(0,0,0));
    }

    jpeg_save_canvas_to_file(canvas, "voronoi.jpeg", 100);
    canvas_free(canvas);
}
