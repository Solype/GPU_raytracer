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

int material_diffuse_compute(struct material_diffuse const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation,
    struct ray *scattered, int *seed)
{

    // struct vec3 scatter_direction;

    compute_random_new_ray_diff(scattered, rec, seed);
    // if (vec3_near_zero(&scatter_direction)) {
    //     scatter_direction = rec->normal;
    // }

    // *scattered = (struct ray){.origin = rec->point, .direction = scatter_direction };
    *attenuation = mat->albedo;
    return 1;
}
