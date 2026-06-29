#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

int *init_fb(int width, int height);
void clear_fb();
void set_pixel(int x, int y);
void cleanup_fb();

#endif // FRAMEBUFFER