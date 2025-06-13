#include "kernel_gpu/vec3.h"
#include "kernel_gpu/struct.h"
#include "kernel_gpu/ray.h"
#include "kernel_gpu/camera.h"

#ifndef INFINITY
    #define INFINITY_BITS 0x7F800000
    #define INFINITY (*(float*)&(unsigned int){INFINITY_BITS})
#endif

int primitive_hits(struct hits *hit, const struct ray *ray, __global const struct primitive *p, struct interval *interval)
{
    switch (p->type) {
    case SHPERE : {
        struct sphere tmp = p->data.sphere;
        return sphere_hits(&tmp, ray, hit, interval);
    }
    default:
        return 1;
    }
    return 0;
}

uchar float_to_uchar(float x) {
    x = fabs(x);
    x = fmin(fmax(x, 0.0f), 1.0f);
    return (uchar)(x * 255.0f + 0.5f);
}

int primitive_list_hits(struct vec3 *dest, const struct ray *ray, __global const struct primitive *primitives, unsigned int nb_primitives)
{
    struct interval interval = (struct interval){
        .tmax = INFINITY,
        .tmin = 0
    };
    struct hits hit = (struct hits){
        .t = -1,
        .point = (struct vec3){.x = 0, .y = 0, .z = 0},
        .normal = (struct vec3){.x = 0, .y = 0, .z = 0},
        .front_face = 0
    };
    struct hits record = hit;
    for (unsigned int i = 0; i < nb_primitives; ++i) {
        if (primitive_hits(&hit, ray, &primitives[i], &interval)
            && (hit.t < record.t || record.t < 0)) {
                record = hit;
        }
    }
    *dest = record.normal;
    dest->x = float_to_uchar(dest->x);
    dest->y = float_to_uchar(dest->y);
    dest->z = float_to_uchar(dest->z);
    return 0;
}

__kernel void compute_ray(
    __global struct vec3 *color,
    __global const struct scene_data *metadata,
    __global const struct primitive *primitives,
    __global const struct material *materials,
    __global const struct light *lights
    )
{
    int id = get_global_id(0);
    struct ray my_ray;
    struct vec3 mycolor = (struct vec3){.x = 0, .y = 0, .z = 0};
    struct vec3 final_color = (struct vec3){.x = 0, .y = 0, .z = 0};
    struct camera cam_copy = metadata->cam;

    int i = id % cam_copy.image_width;
    int j = id / cam_copy.image_width;

    for (int k = 0; k < cam_copy.sample_pixel; k++) {
        camera_compute_ray(&my_ray, &cam_copy, i, j, k);
        primitive_list_hits(&mycolor, &my_ray, primitives, metadata->nb_primitives);
        final_color.x += mycolor.x;
        final_color.y += mycolor.y;
        final_color.z += mycolor.z;
    }
    final_color.x /= cam_copy.sample_pixel;
    final_color.y /= cam_copy.sample_pixel;
    final_color.z /= cam_copy.sample_pixel;
    color[id].x =  final_color.x;
    color[id].y =  final_color.y;
    color[id].z =  final_color.z;
}
