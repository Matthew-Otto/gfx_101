#include <stdint.h>
#include <stdlib.h>
#include "render.h"

#define WIDTH 800
#define HEIGHT 600

int main(void)
{
    if (!init(WIDTH, HEIGHT)) {
        return 1;
    }

    uint32_t* framebuffer = (uint32_t*)malloc(WIDTH * HEIGHT * sizeof(uint32_t));

    bool running = true;
    while (running) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                uint8_t r = (x * 255) / WIDTH;
                uint8_t g = (y * 255) / HEIGHT;
                uint8_t b = 128; // Static blue
                
                // Pack into 32-bit integer: 0x00RRGGBB
                framebuffer[y * WIDTH + x] = (r << 16) | (g << 8) | b;
            }
        }

        // Push pixels to the screen and check if the window was closed
        running = display_update(framebuffer);
    }

    free(framebuffer);
    cleanup();
    return 0;
}