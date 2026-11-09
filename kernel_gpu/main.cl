#include "kernel_gpu/vec3.h"
#include "kernel_gpu/struct.h"
#include "kernel_gpu/ray.h"
#include "kernel_gpu/camera.h"
#include "kernel_gpu/my_random.h"

#ifndef INFINITY
    #define INFINITY_BITS 0x7F800000
    #define INFINITY (*(float*)&(unsigned int){INFINITY_BITS})
#endif

int primitive_hits(struct hits *hit, const struct ray *ray, __global const struct primitive *p, struct interval *interval)
{
    struct primitive primit = *p;
    switch (p->type) {
    case SHPERE : {
        return sphere_hits(&primit, ray, hit, interval);
    }
    default:
        return 0;
    }
    return 0;
}

uchar float_to_uchar(float x) {
    x = fabs(x);
    x = fmin(fmax(x, 0.0f), 1.0f);
    return (uchar)(x * 255.0f + 0.5f);
}

int primitive_list_hits(struct hits *record, const struct ray *ray, __global const struct primitive *primitives, unsigned int nb_primitives)
{
    struct interval interval = (struct interval){
        .tmax = INFINITY,
        .tmin = 0.001f
    };
    struct hits hit = *record;
    int has_hit = 0;

    for (unsigned int i = 0; i < nb_primitives; ++i) {
        if (primitive_hits(&hit, ray, &primitives[i], &interval)) {
            // closest hit so far: later primitives must be nearer
            interval.tmax = hit.t;
            *record = hit;
            has_hit = 1;
        }
    }
    return has_hit;
}

void compute_random_new_ray(struct ray *dest, struct hits *hit, int *rand_seed)
{
    do {
        *rand_seed = (int)(randint(*rand_seed));
        dest->direction.x = ((rand01(*rand_seed) - 0.5f) * 2);

        *rand_seed = (int)(randint(*rand_seed));
        dest->direction.y = ((rand01(*rand_seed) - 0.5f) * 2);

        *rand_seed = (int)(randint(*rand_seed));
        dest->direction.z = ((rand01(*rand_seed) - 0.5f) * 2);
    } while (vec3_dot(&dest->direction, &dest->direction) < 1e-40f);

    if (vec3_dot(&dest->direction, &hit->normal) < 0.0f) {
        dest->direction.x *= -1;
        dest->direction.y *= -1;
        dest->direction.z *= -1;
    }
    vec3_normalize(&dest->direction);
    dest->origin = hit->point;
}

int compute_material(__global struct material const *mats, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation,
    struct ray *scattered, int *seed)
{
    switch (mats[rec->mat_id].type) {
        case DIFFUSE: {
            struct material_diffuse mat = mats[rec->mat_id].data.diffuse;
            return material_diffuse_compute(&mat, r_in, rec, attenuation, scattered, seed);
        }
        case METAL: {
            struct material_metal mat = mats[rec->mat_id].data.metal;
            return material_metal_compute(&mat, r_in, rec, attenuation, scattered, seed);
        }
        case DIELECTRIC: {
            struct material_dielectric mat = mats[rec->mat_id].data.dielectric;
            return material_dielectric_compute(&mat, r_in, rec, attenuation, scattered, seed);
        }
        default:
            break;
    }
    return 1;
}

void init_base_color(struct vec3 *vec, struct ray const *r)
{
    struct vec3 unit = r->direction;
    vec3_normalize(&unit);
    float a = 0.5f * (unit.y + 1);

    *vec = (struct vec3) {
        .x = (1 - a) + (a * 0.5f),
        .y = (1 - a) + (a * 0.7f),
        .z = (1 - a) + (a * 1.0f)
    };
}

void compute_color_ray(
    struct vec3 *dest,
    struct ray *my_ray,
    __global struct scene_data *metadata,
    __global const struct primitive *primitives,
    __global const struct material *materials,
    __global const struct light *lights,
    int *rand_seed
)
{
    struct hits hit = (struct hits){
        .t = -1,
        .point = (struct vec3){.x = 0, .y = 0, .z = 0},
        .normal = (struct vec3){.x = 0, .y = 0, .z = 0},
        .front_face = 0,
        .mat_id = -1
    };
    struct vec3 throughput = (struct vec3){1.0f, 1.0f, 1.0f};

    *dest = (struct vec3){0.0f, 0.0f, 0.0f};
    for (int i = 0; i < 50; ++i) {
        if (!primitive_list_hits(&hit, my_ray, primitives, metadata->nb_primitives)) {
            // ray escaped: sky seen along the LAST bounce direction, not the camera ray
            init_base_color(dest, my_ray);
            dest->x = float_to_uchar(dest->x * throughput.x);
            dest->y = float_to_uchar(dest->y * throughput.y);
            dest->z = float_to_uchar(dest->z * throughput.z);
            return;
        }
        if (!(0 <= hit.mat_id && hit.mat_id < metadata->nb_materials)) {
            return;
        }
        struct ray new_ray = (struct ray){. origin = (struct vec3){ .x = 0.0f, .y = 0.0f, .z = 0.0f}, .direction = (struct vec3){ .x = 0.0f, .y = 0.0f, .z = 0.0f}};
        struct vec3 attenuation = (struct vec3){ .x = 0.0f, .y = 0.0f, .z = 0.0f};
        if (!compute_material(materials, my_ray, &hit, &attenuation, &new_ray, rand_seed)) {
            return;
        }
        *my_ray = new_ray;
        throughput.x *= attenuation.x;
        throughput.y *= attenuation.y;
        throughput.z *= attenuation.z;
    }
    // max depth reached: no light gathered, stays black
}

__kernel void compute_ray(
    __global struct vec3 *color,
    __global struct scene_data *metadata,
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
    // one random stream per pixel, advanced across samples (metadata is read-only)
    int rand_seed = wang_hash(metadata->rand_seed ^ wang_hash(id));

    for (int k = 0; k < cam_copy.sample_pixel; k++) {
        camera_compute_ray(&my_ray, &cam_copy, i, j, rand_seed);
        rand_seed = randint(rand_seed);
        compute_color_ray(&mycolor, &my_ray, metadata, primitives, materials, lights, &rand_seed);
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
    return;
}
