#include "vec3.h"

float3 vec3_load(struct vec3 v)
{
    return (float3)(v.x, v.y, v.z);
}

int vec3_near_zero(float3 v)
{
    return all(fabs(v) < 1e-8f);
}

float3 vec3_random_unit(uint *seed)
{
    float z = rand01(seed) * 2.0f - 1.0f;
    float a = rand01(seed) * 2.0f * M_PI_F;
    float r = sqrt(fmax(0.0f, 1.0f - z * z));

    return (float3)(r * cos(a), r * sin(a), z);
}

float3 vec3_reflect(float3 v, float3 n)
{
    return v - 2.0f * dot(v, n) * n;
}

// uv and n must be unit vectors, n facing against uv
float3 vec3_refract(float3 uv, float3 n, float etai_over_etat)
{
    float cos_theta = fmin(dot(-uv, n), 1.0f);
    float3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    float3 r_out_parallel = -sqrt(fmax(0.0f, 1.0f - dot(r_out_perp, r_out_perp))) * n;

    return r_out_perp + r_out_parallel;
}
