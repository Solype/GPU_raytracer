#include "camera.h"

void create_camera(struct camera *cam, int width, int height, float focal)
{
    cam->focal_length = focal;
    cam->view_port_height = 2;
    cam->view_port_width = cam->view_port_height * ((float)width / (float)height);
    cam->center = (struct vec3){0.0f, 0.0f, 0.0f};
    cam->view_port_u = (struct vec3){cam->view_port_width, 0, 0};
    cam->view_port_v = (struct vec3){0, -(cam->view_port_height), 0};
    cam->image_height = height;
    cam->image_width = width;

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
