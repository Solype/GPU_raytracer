#include "materials.h"
#include "ray.h"
#include "hits.h"

int material_diffuse_scatter(struct material_diffuse const *mat, struct hits const *rec,
    float3 *attenuation, struct ray *scattered, uint *seed)
{
    float3 direction = rec->normal + vec3_random_unit(seed);

    if (vec3_near_zero(direction)) {
        direction = rec->normal;
    }
    scattered->origin = rec->point;
    scattered->direction = normalize(direction);
    *attenuation = vec3_load(mat->albedo);
    return 1;
}

int material_metal_scatter(struct material_metal const *mat, struct ray const *r_in, struct hits const *rec,
    float3 *attenuation, struct ray *scattered, uint *seed)
{
    // fuzz is relative to a unit reflection
    float3 reflected = normalize(vec3_reflect(r_in->direction, rec->normal));
    float3 fuzz = vec3_random_unit(seed);

    if (dot(fuzz, rec->normal) < 0.0f) {
        fuzz = -fuzz;
    }
    scattered->origin = rec->point;
    scattered->direction = normalize(reflected + mat->fuzz * fuzz);
    *attenuation = vec3_load(mat->albedo);
    // absorbed when the fuzzed ray goes under the surface
    return dot(scattered->direction, rec->normal) > 0.0f;
}

// Schlick approximation
static float reflectance(float cosine, float refraction_index)
{
    float r0 = (1.0f - refraction_index) / (1.0f + refraction_index);

    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pown(1.0f - cosine, 5);
}

int material_dielectric_scatter(struct material_dielectric const *mat, struct ray const *r_in, struct hits const *rec,
    float3 *attenuation, struct ray *scattered, uint *seed)
{
    float ri = rec->front_face ? (1.0f / mat->refraction_index) : mat->refraction_index;
    float3 unit_direction = normalize(r_in->direction);
    float cos_theta = fmin(dot(-unit_direction, rec->normal), 1.0f);
    float sin_theta = sqrt(fmax(0.0f, 1.0f - cos_theta * cos_theta));
    int cannot_refract = ri * sin_theta > 1.0f;

    scattered->origin = rec->point;
    if (cannot_refract || reflectance(cos_theta, ri) > rand01(seed)) {
        scattered->direction = vec3_reflect(unit_direction, rec->normal);
    } else {
        scattered->direction = vec3_refract(unit_direction, rec->normal, ri);
    }
    scattered->direction = normalize(scattered->direction);
    *attenuation = (float3)(1.0f);
    return 1;
}
