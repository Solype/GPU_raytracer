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

int material_diffuse(struct material_diffuse const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation, struct ray *scattered)
{

    struct vec3 scatter_direction;
    int seed = hash_seed_from_floats(&rec->normal, &r_in->direction);

    scatter_direction.x = (rand01(seed) - 0.5f) * 2;
    seed = randint(seed);

    scatter_direction.y = (rand01(seed) - 0.5f) * 2;
    seed = randint(seed);

    scatter_direction.z = (rand01(seed) - 0.5f) * 2;

    if (vec3_near_zero(&scatter_direction)) {
        scatter_direction = rec->normal;
    }

    *scattered = (struct ray){.origin = rec->point, .direction = scatter_direction };
    *attenuation = mat->albedo;
    return 1;
}
