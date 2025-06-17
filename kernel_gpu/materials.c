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
    #include "kernel_gpu/materials.h"
#endif

int material_diffuse_compute(struct material_diffuse const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation,
    struct ray *scattered, int *seed)
{
    vec3_random_unit(&scattered->direction, seed);
    if (vec3_dot(&scattered->direction, &rec->normal) < 0.0f) {
        scattered->direction.x *= -1;
        scattered->direction.y *= -1;
        scattered->direction.z *= -1;
    }
    vec3_normalize(&scattered->direction);
    scattered->origin = rec->point;
    *attenuation = mat->albedo;
    return 1;
}

int material_metal_compute(struct material_metal const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation,
    struct ray *scattered, int *seed)
{
    struct vec3 unit_vec;
    float fuzz = clamp(mat->fuzz, 0.0f, 1.0f);
    vec3_random_unit(&unit_vec, seed);
    vec3_reflect(&scattered->direction, &r_in->direction, &rec->normal);
    unit_vec.x *= fuzz;
    unit_vec.y *= fuzz;
    unit_vec.z *= fuzz;
    scattered->direction.x += unit_vec.x;
    scattered->direction.y += unit_vec.y;
    scattered->direction.z += unit_vec.z;

    scattered->origin = rec->point;
    *attenuation = mat->albedo;
    vec3_normalize(&scattered->direction);
    return vec3_dot(&scattered->direction, &rec->normal) > 0;
}

