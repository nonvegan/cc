#include "./canvas.h"
#include "./yuv.h"
#include <math.h>

#define WIDTH_PX 256
#define HEIGHT_PX 256
#define RADIUS_PX (WIDTH_PX / 3)

#define BG_COLOR 0x282C34
#define FG_COLOR 0xF92672
#define AA_X 4

#define VIDEO_DURATION 10
#define FPS 30

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

    Y4m2 *y4m2 = y4m2_open_video("circle.y4m", WIDTH_PX, HEIGHT_PX, FPS);

    size_t frame_count = FPS * VIDEO_DURATION;
    for(size_t i = 0; i < frame_count; i++) {
        canvas_clear(canvas, BG_COLOR);
        canvas_draw_anti_aliased_filled_circle(canvas, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f,
                                               fabs(cos(i/25.0f)) * RADIUS_PX, FG_COLOR, BG_COLOR);
        y4m2_dump_canvas_frame(y4m2, canvas, 1);
        printf("Generating %s: %.0f%%\r", y4m2->file_name, i * 100.0f / frame_count);
        fflush(stdout);
    }
    printf("Generated %s       \n", y4m2->file_name);
    y4m2_close_video(y4m2);

    canvas_free(canvas);
    return 0;
}
