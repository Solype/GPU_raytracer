#include "struct.h"
#include "ray.h"
#include "hits.h"
#include "interval.h"

#define MAX_DEPTH 50

struct ray camera_get_ray(struct camera const *cam, int i, int j, uint *seed)
{
    // random point inside the pixel square, for antialiasing
    float u = (float)i + rand01(seed) - 0.5f;
    float v = (float)j + rand01(seed) - 0.5f;
    float3 center = vec3_load(cam->center);
    float3 pixel = vec3_load(cam->pixel00_loc)
        + u * vec3_load(cam->pixel_delta_u)
        + v * vec3_load(cam->pixel_delta_v);

    return (struct ray){ .origin = center, .direction = normalize(pixel - center) };
}

int scene_hits(struct ray const *ray, __global struct primitive const *primitives, uint nb_primitives, struct hits *rec)
{
    struct interval interval = { .tmin = 0.001f, .tmax = INFINITY };
    int has_hit = 0;

    for (uint i = 0; i < nb_primitives; ++i) {
        struct primitive p = primitives[i];
        int hit = 0;

        switch (p.type) {
        case SPHERE:
            hit = sphere_hits(&p.data.sphere, ray, &interval, rec);
            break;
        default:
            break;
        }
        if (hit) {
            // closest hit so far: later primitives must be nearer
            interval.tmax = rec->t;
            rec->mat_id = p.mat_id;
            has_hit = 1;
        }
    }
    return has_hit;
}

int material_scatter(__global struct material const *materials, struct ray const *r_in, struct hits const *rec,
    float3 *attenuation, struct ray *scattered, uint *seed)
{
    struct material mat = materials[rec->mat_id];

    switch (mat.type) {
    case DIFFUSE:
        return material_diffuse_scatter(&mat.data.diffuse, rec, attenuation, scattered, seed);
    case METAL:
        return material_metal_scatter(&mat.data.metal, r_in, rec, attenuation, scattered, seed);
    case DIELECTRIC:
        return material_dielectric_scatter(&mat.data.dielectric, r_in, rec, attenuation, scattered, seed);
    default:
        return 0;
    }
}

float3 sky_color(float3 direction)
{
    float a = 0.5f * (normalize(direction).y + 1.0f);

    return (1.0f - a) * (float3)(1.0f) + a * (float3)(0.5f, 0.7f, 1.0f);
}

// linear color carried by one camera ray
float3 ray_color(struct ray ray, __global struct scene_data const *scene,
    __global struct primitive const *primitives, __global struct material const *materials, uint *seed)
{
    float3 throughput = (float3)(1.0f);
    struct hits rec;

    for (int depth = 0; depth < MAX_DEPTH; ++depth) {
        if (!scene_hits(&ray, primitives, scene->nb_primitives, &rec)) {
            // ray escaped: sky seen along the LAST bounce direction
            return throughput * sky_color(ray.direction);
        }
        if (rec.mat_id >= scene->nb_materials) {
            return (float3)(0.0f);
        }
        float3 attenuation;
        struct ray scattered;
        if (!material_scatter(materials, &ray, &rec, &attenuation, &scattered, seed)) {
            return (float3)(0.0f);
        }
        throughput *= attenuation;
        ray = scattered;
    }
    // max depth reached: no light gathered
    return (float3)(0.0f);
}

__kernel void compute_ray(
    __global struct vec3 *color,
    __global struct scene_data const *scene,
    __global struct primitive const *primitives,
    __global struct material const *materials)
{
    uint id = get_global_id(0);
    struct camera cam = scene->cam;
    int i = id % cam.image_width;
    int j = id / cam.image_width;
    // one random stream per pixel, advanced across samples
    uint seed = wang_hash(scene->rand_seed ^ wang_hash(id));
    float3 sum = (float3)(0.0f);

    if (seed == 0) {
        seed = 1;
    }
    for (int k = 0; k < cam.sample_pixel; ++k) {
        sum += ray_color(camera_get_ray(&cam, i, j, &seed), scene, primitives, materials, &seed);
    }
    // gamma 2 on the averaged linear color, never per sample
    float3 c = sqrt(clamp(sum / (float)cam.sample_pixel, 0.0f, 1.0f)) * 255.0f + 0.5f;
    color[id] = (struct vec3){ c.x, c.y, c.z };
}
