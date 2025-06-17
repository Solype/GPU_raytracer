#ifdef __cmakelist
    #include "vec3.h"
    #include "ray.h"
    #include "hits.h"
    #include "interval.h"
    #include "material_diffuse.h"
#else
    #include "kernel_gpu/vec3.h"
    #include "kernel_gpu/ray.h"
    #include "kernel_gpu/hits.h"
    #include "kernel_gpu/interval.h"
    #include "kernel_gpu/material_diffuse.h"
#endif

void compute_random_new_ray_diff(struct ray *dest, struct hits const *hit, int *rand_seed)
{
    vec3_random_unit(&dest->direction, rand_seed);
    if (vec3_dot(&dest->direction, &hit->normal) < 0.0f) {
        dest->direction.x *= -1;
        dest->direction.y *= -1;
        dest->direction.z *= -1;
    }
    vec3_normalize(&dest->direction);
    dest->origin = hit->point;
}

int material_diffuse_compute(struct material_diffuse const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation,
    struct ray *scattered, int *seed)
{
    compute_random_new_ray_diff(scattered, rec, seed);
    *attenuation = mat->albedo;
    return 1;
}
