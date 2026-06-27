#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include <stdbool.h>

bool init(int width, int height);
void cleanup();
bool display_update(const uint32_t* framebuffer);

#endif // RENDER_H