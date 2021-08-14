#include "./canvas.h"

#define WIDTH_PX 256
#define HEIGHT_PX 256
#define RADIUS_PX (WIDTH_PX / 3)

#define BG_COLOR 0x282C34
#define FG_COLOR 0xF92672
#define AA_X 4

int main(void)
{
    Canvas *canvas = canvas_create(WIDTH_PX, HEIGHT_PX);

    canvas_clear(canvas, BG_COLOR); 
    canvas_draw_circle(canvas, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f, RADIUS_PX, FG_COLOR);
    canvas_save_to_ppm(canvas, "circle.ppm");

    canvas_clear(canvas, BG_COLOR); 
    canvas_draw_filled_circle(canvas, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f, RADIUS_PX, FG_COLOR);
    canvas_save_to_ppm(canvas, "filled_circle.ppm");

    canvas_clear(canvas, BG_COLOR); 
    canvas_draw_anti_aliased_filled_circle(canvas, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f, RADIUS_PX, FG_COLOR, BG_COLOR);
    canvas_save_to_ppm(canvas, "anti_aliased_filled_circle.ppm");

    canvas_free(canvas);
    return 0;
}
