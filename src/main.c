#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
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
void sphere(coordinate *points) {
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
            //y = radius * sin(theta) * sin(phi);
            y = radius * cos(theta);
            //z = radius * cos(theta) + distance;
            z = radius * sin(theta) * sin(phi);
            points[pt_idx++] = (coordinate){x,y,z};

            theta += PI / theta_steps; 
        }
        phi += (2*PI) / phi_steps;
    }
}

// translates point 3d space
bool translate(float x, float y, float z, const coordinate* base, coordinate* translated) {
    translated->x = base->x + x;
    translated->y = base->y + y;
    translated->z = base->z + z;

    // bound checking
    if (translated->z < 0) {
        translated = NULL;
        return false;
    }
    return true;
}

// rotates a point about the specified origin from -1 to 1 (-360 to 360)
bool rotate(float x_orig, float y_orig, float z_orig,
            float x_rot, float y_rot, float z_rot,
            const coordinate* base, coordinate* rotated) {
    
    rotated->x = base->x;
    rotated->y = base->y;
    rotated->z = base->z;

    // find position from origin
    float x_dist = base->x - x_orig;
    float y_dist = base->y - y_orig;
    float z_dist = base->z - z_orig;

    //rotate x
    if (x_rot != 0) {
        float theta_x = atan(z_dist / y_dist);
        float r_x = sqrt(pow(z_dist,2) + pow(y_dist,2));
        float theta_rot = theta_x + (x_rot * 2 * PI);
        rotated->y = r_x * sin(theta_rot);
        rotated->z = r_x * cos(theta_rot);
    }
    //rotate y
    if (y_rot != 0) {
        float theta_y = atan(x_dist / z_dist);
        float r_y = sqrt(pow(x_dist,2) + pow(z_dist,2));
        float theta_rot = theta_y + (y_rot * 2 * PI);
        rotated->z = r_y * sin(theta_rot);
        rotated->x = r_y * cos(theta_rot);
    }
    //rotate z
    if (z_rot != 0) {
        float theta_z = atan(y_dist / x_dist);
        float r_z = sqrt(pow(y_dist,2) + pow(x_dist,2));
        float theta_rot = theta_z + (z_rot * 2 * PI);
        rotated->x = r_z * sin(theta_rot);
        rotated->y = r_z * cos(theta_rot);
    }

    return true; // todo out of bounds check
}

// convert 3d coordinate to 2d x,y projection
bool project(float dist, float fov, const coordinate* point, projection* proj) {
    float scale = point->z + dist;
    if (scale < 0.001) return false;

    proj->x = (point->x * fov) / scale;
    proj->y = (point->y * fov) / scale;
    return true;
}

// takes a 2d projection with coordinates (-1..1) and scales it to screen (pixel) dimensions
bool screen(const projection *p, pixel *pxl) {
    const float x_center = WIDTH/2;
    const float y_center = HEIGHT/2;
    const float min_dim = HEIGHT < WIDTH ? HEIGHT : WIDTH;

    pxl->x = p->x*min_dim/2+x_center;
    if (pxl->x < 0 || pxl->x >= WIDTH) return false;
    pxl->y = -1*p->y*min_dim/2+y_center;
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
    sphere(points);


    bool running = true;

    coordinate origin = {0,0,0};
    coordinate rot = {0,0,0};

    while (running) {
        clear_fb();
        float distance = 1;


        coordinate translated;
        coordinate rotated;
        projection p;
        pixel pxl;

        for (int i = 0; i < 861; i++){
            if (!rotate(origin.x,origin.y,origin.z, rot.x,rot.y,rot.z, &points[i], &rotated)) continue;
            if (!translate(origin.x,origin.y,origin.z, &rotated, &translated)) continue;
            if (!project(distance, 1, &translated, &p)) continue;
            if (!screen(&p, &pxl)) continue;
            set_pixel(pxl.x, pxl.y);
        }

        // Push pixels to the screen and check if the window was closed
        running = display_update(framebuffer);

        rot.y += 0.001;
    }

    cleanup_fb();
    cleanup();
    return 0;
}