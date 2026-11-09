#include <math.h>
#include "camera.h"

#define CAMERA_PI 3.14159265358979f

static struct vec3 v_add(struct vec3 a, struct vec3 b)
{
    return (struct vec3){ a.x + b.x, a.y + b.y, a.z + b.z };
}

static struct vec3 v_sub(struct vec3 a, struct vec3 b)
{
    return (struct vec3){ a.x - b.x, a.y - b.y, a.z - b.z };
}

static struct vec3 v_scale(struct vec3 v, float s)
{
    return (struct vec3){ v.x * s, v.y * s, v.z * s };
}

static struct vec3 v_cross(struct vec3 a, struct vec3 b)
{
    return (struct vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static struct vec3 v_unit(struct vec3 v)
{
    return v_scale(v, 1.0f / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

static float deg_to_rad(float degrees)
{
    return degrees * CAMERA_PI / 180.0f;
}

void camera_init(struct camera *cam)
{
    float viewport_height = 2.0f * tanf(deg_to_rad(cam->vfov) / 2.0f) * cam->focus_dist;
    float viewport_width = viewport_height * ((float)cam->image_width / (float)cam->image_height);
    float defocus_radius = cam->focus_dist * tanf(deg_to_rad(cam->defocus_angle) / 2.0f);

    // camera frame: w points backward, u right, v up
    struct vec3 w = v_unit(v_sub(cam->lookfrom, cam->lookat));
    struct vec3 u = v_unit(v_cross(cam->vup, w));
    struct vec3 v = v_cross(w, u);
    struct vec3 viewport_u = v_scale(u, viewport_width);
    struct vec3 viewport_v = v_scale(v, -viewport_height);  // image rows go down

    cam->center = cam->lookfrom;
    cam->pixel_delta_u = v_scale(viewport_u, 1.0f / (float)cam->image_width);
    cam->pixel_delta_v = v_scale(viewport_v, 1.0f / (float)cam->image_height);

    struct vec3 upper_left = v_sub(
        v_sub(v_sub(cam->center, v_scale(w, cam->focus_dist)), v_scale(viewport_u, 0.5f)),
        v_scale(viewport_v, 0.5f));
    cam->pixel00_loc = v_add(upper_left, v_scale(v_add(cam->pixel_delta_u, cam->pixel_delta_v), 0.5f));

    cam->defocus_disk_u = v_scale(u, defocus_radius);
    cam->defocus_disk_v = v_scale(v, defocus_radius);
}
