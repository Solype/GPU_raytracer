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
    vec3_add(&scattered->direction, &rec->normal);

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
    struct vec3 save_reflected;
    float dot = 0;

    vec3_reflect(&reflected, &r_in->direction, &rec->normal);
    save_reflected = reflected;
    vec3_random_unit(&unit_vec, seed);
    if (vec3_dot(&unit_vec, &rec->normal) < 0) {
        vec3_mul(&unit_vec, -1);
    }
    vec3_mul(&unit_vec, fuzz);
    vec3_add(&reflected, &unit_vec);
    vec3_normalize(&reflected);
    dot = vec3_dot(&reflected, &rec->normal);
    scattered->origin = rec->point;
    scattered->direction = reflected;
    *attenuation = mat->albedo;
    return 1;
}

static float reflectance(float cosine, float refraction_index) {
        // Use Schlick's approximation for reflectance.
        float r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
}

int material_dielectric_compute(struct material_dielectric const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation,
    struct ray *scattered, int *seed)
{
    *attenuation = (struct vec3){1.0f, 1.0f, 1.0f};
    float ri = rec->front_face == 1 ? (1.0f / mat->refraction_index) : mat->refraction_index;

    struct vec3 unit_direction = r_in->direction;
    vec3_normalize(&unit_direction);
    struct vec3 inverted_unit_direction = unit_direction;
    vec3_mul(&inverted_unit_direction, -1);

    float cos_theta = fmin(vec3_dot(&inverted_unit_direction, &rec->normal), 1.0f);
    float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

    int cannot_refract = ri * sin_theta > 1.0f;
    struct vec3 direction;

    if (cannot_refract || reflectance(cos_theta, ri) > *seed) {
        vec3_reflect(&direction, &unit_direction, &rec->normal);
    } else {
        vec3_refract(&direction, &unit_direction, &rec->normal, ri);
    }

    vec3_normalize(&direction);
    scattered->direction = direction;
    scattered->origin = rec->point;
    return 1;
}
