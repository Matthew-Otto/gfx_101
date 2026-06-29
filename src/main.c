#include <stdint.h>
#include <math.h>
#include "render.h"
#include "framebuffer.h"

#define WIDTH 640
#define HEIGHT 480
const double PI = 3.14159265358979323846;

typedef struct {
    float x;
    float y;
    float z;
} coordinate;

typedef struct {
    float x;
    float y;
} projection;

typedef struct {
    int x;
    int y;
} pixel;

// draw a sphere
void sphere(float distance, coordinate *points) {
    const float radius = 1;
    const int theta_steps = 20;
    const int phi_steps = 40;

    int pt_idx = 0;
    float x,y,z;
    
    float phi = 0;
    for (int i = 0; i <= phi_steps; i++) {
        float theta = 0;
        for (int j=0; j <= theta_steps; j++) {
            x = radius * sin(theta) * cos(phi);
            y = radius * sin(theta) * sin(phi);
            z = radius * cos(theta) + distance;
            points[pt_idx++] = (coordinate){x,y,z};

            theta += PI / theta_steps; 
        }
        phi += (2*PI) / phi_steps;
    }
}


// convert 3d coordinate to 2d x,y projection
bool project(float dist, float fov, coordinate point, projection* proj) {
    float scale = point.z + dist;
    if (scale < 0.001) return false;

    proj->x = (point.x * fov) / scale;
    proj->y = (point.y * fov) / scale;
    return true;
}

// takes a 2d projection with coordinates (-1..1) and scales it to screen pixel dimensions
bool scale(projection *p, pixel *pxl) {
    const int x_center = WIDTH/2;
    const int y_center = HEIGHT/2;

    pxl->x = p->x*x_center+x_center;
    if (pxl->x < 0 || pxl->x >= WIDTH) return false;
    pxl->y = -1*p->y*y_center+y_center;
    if (pxl->y < 0 || pxl->y >= HEIGHT) return false;
    
    return true;
}

int main(void)
{
    if (!init(WIDTH, HEIGHT)) {
        return 1;
    }

    int* framebuffer = init_fb(WIDTH, HEIGHT);
    coordinate points[861];
    sphere(1, points);


    bool running = true;
    while (running) {
        float distance = 1;


        projection p;
        pixel pxl;

        for (int i = 0; i < 861; i++){
            if (!project(distance, 1, points[i], &p)) continue;
            if (!scale(&p, &pxl)) continue;
            set_pixel(pxl.x, pxl.y);
        }

        // Push pixels to the screen and check if the window was closed
        running = display_update(framebuffer);
    }

    cleanup_fb();
    cleanup();
    return 0;
}