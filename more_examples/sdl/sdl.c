#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "../../src/canvas.h"
#include "../../src/vec.h"
#include "../../src/jpeg.h"

#define WIDTH_PX 620
#define HEIGHT_PX 480
#define FPS 30

uint32_t rand_gray_shade()
{
    int rand_255 = rand() % 256;
    return RGB(rand_255, rand_255, rand_255);
}

int main()
{
    srand(time(0));

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "ERROR: Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("White Noise", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WIDTH_PX, HEIGHT_PX, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        fprintf(stderr, "ERROR: Failed to create SDL Window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "ERROR: Failed to create SDL Renderer: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, WIDTH_PX, HEIGHT_PX);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        fprintf(stderr, "ERROR: Failed to create SDL Texture: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    Canvas *canvas = canvas_create(WIDTH_PX, HEIGHT_PX);

    int quit = 0;
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                        case SDLK_q:
                            quit = 1;
                            break;
                    }
                    break;
            }
        }

        canvas_map(canvas, rand_gray_shade);
        if (SDL_UpdateTexture(texture, NULL, canvas->ctx, 3 * canvas->width)) {
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyTexture(texture);
            canvas_free(canvas);
            fprintf(stderr, "ERROR: Failed to update SDL Texture: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay((int)(1.0f / FPS * 1000));
        //TODO(#9): Proper frametime without blocking
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();

    canvas_free(canvas);
    return EXIT_SUCCESS;
}
