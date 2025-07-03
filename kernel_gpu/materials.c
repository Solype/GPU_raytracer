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
    float fuzz = mat->fuzz;
    struct vec3 unit_vec;
    struct vec3 reflected;

    vec3_reflect(&reflected, &r_in->direction, &rec->normal);
    
    vec3_random_unit(&unit_vec, seed);
    vec3_mul(&unit_vec, fuzz);
    vec3_add(&reflected, &unit_vec);

    scattered->origin = rec->point;
    scattered->direction = reflected;
    *attenuation = mat->albedo;
    vec3_normalize(&scattered->direction);
    return vec3_dot(&scattered->direction, &rec->normal) > 0;
}

int material_dielectric_compute(struct material_dielectric const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation,
    struct ray *scattered, int *seed)
{
    attenuation->x = 1.0f;
    attenuation->y = 1.0f;
    attenuation->z = 1.0f;

    float ri = rec->front_face == 1 ? (1.0f / mat->refraction_index) : mat->refraction_index;
    struct vec3 unit_direction = r_in->direction;
    struct vec3 refracted;
    vec3_normalize(&unit_direction);
    vec3_refract(&refracted, &unit_direction, &rec->normal, ri);
    scattered->direction = refracted;
    scattered->origin = rec->point;
    return 1;
}
