#include "render.h"
#include <SDL3/SDL.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static int fb_width = 0;
static int fb_height = 0;

bool init(int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return false;
    }

    fb_width = width;
    fb_height = height;

    if (!SDL_CreateWindowAndRenderer("gfx_101", width, height, 0, &window, &renderer)) {
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, 
                                SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        return false;
    }

    return true;
}

bool display_update(const int* framebuffer) {
    // 1. Pump events to keep the OS from marking the window as "Not Responding"
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return false; // Signal main.c to break its loop
        }
    }

    // 2. Blast the pixels to the screen
    SDL_UpdateTexture(texture, NULL, framebuffer, fb_width * sizeof(int));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    return true;
}

void cleanup(void) {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}