#ifdef __cmakelist
    #include "camera.h"
    #include "my_random.h"
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>

#else
    #include "kernel_gpu/camera.h"
    #include "kernel_gpu/my_random.h"
#endif

void camera_compute_ray(struct ray *r, struct camera const *cam, int i, int j, int k)
{
    float x = rand01(k) - 0.5f;
    float y = rand01(k + 1) - 0.5f;
    float i_f = (float)i;
    float j_f = (float)j;
    struct vec3 pixel_center;

    pixel_center.x = cam->pixel00_loc.x + (i_f + x) * cam->pixel_delta_u.x + (j_f + y) * cam->pixel_delta_v.x;
    pixel_center.y = cam->pixel00_loc.y + (i_f + x) * cam->pixel_delta_u.y + (j_f + y) * cam->pixel_delta_v.y;
    pixel_center.z = cam->pixel00_loc.z + (i_f + x) * cam->pixel_delta_u.z + (j_f + y) * cam->pixel_delta_v.z;

    r->origin = cam->center;
    r->direction.x = pixel_center.x - cam->center.x;
    r->direction.y = pixel_center.y - cam->center.y;
    r->direction.z = pixel_center.z - cam->center.z;
}

void create_camera(struct camera *cam, int width, int height, float focal)
{
    cam->focal_length = focal;
    cam->view_port_height = 2;
    cam->view_port_width = cam->view_port_height * ((float)width / (float)height);
    cam->center = (struct vec3){0, 0, 0};
    cam->view_port_u = (struct vec3){cam->view_port_width, 0, 0};
    cam->view_port_v = (struct vec3){0, -(cam->view_port_height), 0};
    cam->image_height = height;
    cam->image_width = width;
    cam->sample_pixel = 100;

    cam->pixel_delta_u = (struct vec3){
        .x = cam->view_port_u.x / (float)width,
        .y = cam->view_port_u.y / (float)width,
        .z = cam->view_port_u.z / (float)width
    };
    cam->pixel_delta_v = (struct vec3){
        .x = cam->view_port_v.x / (float)height,
        .y = cam->view_port_v.y / (float)height,
        .z = cam->view_port_v.z / (float)height
    };
    cam->view_port_upperleft = (struct vec3){
        .x = cam->center.x - 0.5f * (cam->view_port_u.x + cam->view_port_v.x),
        .y = cam->center.y - 0.5f * (cam->view_port_u.y + cam->view_port_v.y),
        .z = cam->center.z - 0.5f * (cam->view_port_u.z + cam->view_port_v.z) - focal
    };
    cam->pixel00_loc = (struct vec3){
        .x = cam->view_port_upperleft.x + 0.5f * (cam->pixel_delta_u.x + cam->pixel_delta_v.x),
        .y = cam->view_port_upperleft.y + 0.5f * (cam->pixel_delta_u.y + cam->pixel_delta_v.y),
        .z = cam->view_port_upperleft.z + 0.5f * (cam->pixel_delta_u.z + cam->pixel_delta_v.z),
    };
}

void color_hit(struct vec3 *color, struct hits const *b)
{
    color->x = b->normal.x * 0.5f + 0.5f;
    color->y = b->normal.y * 0.5f + 0.5f;
    color->z = b->normal.x * 0.5f + 0.5f;
}
