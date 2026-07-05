#include "render.h"
#include <SDL3/SDL.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static int fb_width = 0;
static int fb_height = 0;

static uint64_t last_time = 0;
#define TARGET_FPS 60
#define TARGET_FRAME_TIME_MS (1000 / TARGET_FPS)

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

    last_time = SDL_GetTicks();
    return true;
}

bool display_update(const int* framebuffer) {
    // Pump events to keep the OS from marking the window as "Not Responding"
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return false; // Signal main.c to break its loop
        }
    }

    // Update screen
    SDL_UpdateTexture(texture, NULL, framebuffer, fb_width * sizeof(int));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Frame pacing
    uint64_t current_time = SDL_GetTicks();
    uint64_t elapsed_time = current_time - last_time;

    if (elapsed_time < TARGET_FRAME_TIME_MS) {
        SDL_Delay(TARGET_FRAME_TIME_MS - elapsed_time);
    }
    last_time = SDL_GetTicks();

    return true;
}

void cleanup(void) {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}