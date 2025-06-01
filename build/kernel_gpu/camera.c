#ifdef __cmakelist
    #include "camera.h"
    #include <stdlib.h>
#else
    #include "kernel_gpu/camera.h"
    #define RAND_MAX 3.402823466e+38f
    
float rand()
{
    int id = get_global_id(0);
    int seed = (int)(void*)&id;

    seed = (seed << 13) ^ seed;
    seed = (1.0f - ((seed * (seed * 15731u + 1376312589u)) & 0x7fffffff));
    float seed_f = (seed < 0) ? seed * -1 : seed;
    return seed;
}
#endif

void camera_compute_ray(struct ray *r, struct camera const *cam, int i, int j)
{
    float x = ((float)rand()) / RAND_MAX + 1 - 0.5f;
    float y = ((float)rand()) / RAND_MAX + 1 - 0.5f;
    struct vec3 pixel_center;
    struct vec3 ray_dir;
    float i_f = (float)i;
    float j_f = (float)j;

    pixel_center.x = cam->pixel00_loc.x + (i_f + x) * cam->pixel_delta_u.x + j_f * cam->pixel_delta_v.x;
    pixel_center.y = cam->pixel00_loc.y + (i_f + x) * cam->pixel_delta_u.y + j_f * cam->pixel_delta_v.y;
    pixel_center.z = cam->pixel00_loc.z + (i_f + x) * cam->pixel_delta_u.z + j_f * cam->pixel_delta_v.z;

    ray_dir.x = pixel_center.x - cam->center.x;
    ray_dir.y = pixel_center.y - cam->center.y;
    ray_dir.z = pixel_center.z - cam->center.z;

    r->direction = ray_dir;
    r->origin = cam->center;
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
    cam->sample_pixel = 20;

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
