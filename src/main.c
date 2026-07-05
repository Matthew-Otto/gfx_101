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
            y = radius * cos(theta);
            z = radius * sin(theta) * sin(phi);
            points[pt_idx++] = (coordinate){x,y,z};

            theta += PI / theta_steps; 
        }
        phi += (2*PI) / phi_steps;
    }
}

// rotates a point about the specified origin from -1 to 1 (-360 to 360)
bool rotate(float x_rot, float y_rot, float z_rot,
            const coordinate* base, coordinate* rotated) {
    
    // Convert rotation float (-1 to 1) to radians (-2PI to 2PI)
    float rx = x_rot * 2 * PI;
    float ry = y_rot * 2 * PI;
    float rz = z_rot * 2 * PI;

    float x = base->x;
    float y = base->y;
    float z = base->z;

    // Rotate around X axis
    if (x_rot != 0) {
        float temp_y = y * cos(rx) - z * sin(rx);
        float temp_z = y * sin(rx) + z * cos(rx);
        y = temp_y;
        z = temp_z;
    }

    // Rotate around Y axis
    if (y_rot != 0) {
        float temp_x = x * cos(ry) + z * sin(ry);
        float temp_z = -x * sin(ry) + z * cos(ry);
        x = temp_x;
        z = temp_z;
    }

    // Rotate around Z axis
    if (z_rot != 0) {
        float temp_x = x * cos(rz) - y * sin(rz);
        float temp_y = x * sin(rz) + y * cos(rz);
        x = temp_x;
        y = temp_y;
    }

    rotated->x = x;
    rotated->y = y;
    rotated->z = z;

    return true;
}

// translates point in 3D space
bool translate(float x, float y, float z, const coordinate* base, coordinate* translated) {
    translated->x = base->x + x;
    translated->y = base->y + y;
    translated->z = base->z + z;

    // bounds checking
    if (translated->z < 0) return false;
    return true;
}

// convert 3d coordinate to 2d x,y projection
bool project(float fov_degrees, const coordinate* point, projection* proj) {  
    // Prevent division by zero and clipping behind the camera
    if (point->z < 0.1) return false; 

    // Convert FOV to radians and calculate the perspective scaling factor
    float fov_rad = fov_degrees * (PI / 180.0);
    float fov_scale = 1.0 / tan(fov_rad / 2.0);

    // Apply the true perspective projection
    proj->x = (point->x * fov_scale) / point->z;
    proj->y = (point->y * fov_scale) / point->z;
    
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

    coordinate origin = {0,0,5};
    coordinate rot = {0,0,0};

    while (running) {
        clear_fb();
        float fov = 30;


        coordinate translated;
        coordinate rotated;
        projection p;
        pixel pxl;

        for (int i = 0; i < 861; i++){
            if (!rotate(rot.x,rot.y,rot.z, &points[i], &rotated)) continue;
            if (!translate(origin.x,origin.y,origin.z, &rotated, &translated)) continue;
            if (!project(fov, &translated, &p)) continue;
            if (!screen(&p, &pxl)) continue;
            set_pixel(pxl.x, pxl.y);
        }

        // Push pixels to the screen and check if the window was closed
        running = display_update(framebuffer);

        rot.x -= 0.001;
        rot.y += 0.002;
        rot.z += 0.002;
    }

    cleanup_fb();
    cleanup();
    return 0;
}