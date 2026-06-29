#include <stdlib.h>

int* framebuffer;
int width;
int height;

int *init_fb(int max_width, int max_height) {
    width = max_width;
    height = max_height;
    framebuffer = (int*)malloc(width * height * sizeof(int));
    return framebuffer;
}

void clear_fb() {
}

void set_pixel(int x, int y) {
    if (0 <= x && x < width && 0 <= y && y < height) {
        framebuffer[y*width+x] = 0xFFFFFF;
    }
}

void cleanup_fb() {
    free(framebuffer);
}