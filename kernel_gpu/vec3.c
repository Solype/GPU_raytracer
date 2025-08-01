#include "vec3.h"

#ifdef __cmakelist
    #include <math.h>
    #include <stdio.h>
    #include "my_random.h"
#else
    #include "kernel_gpu/my_random.h"
#endif

void vec3_add(struct vec3 *dest, const struct vec3 *source)
{
    dest->x += source->x;
    dest->y += source->y;
    dest->z += source->z;
}

void vec3_sub(struct vec3 *dest, const struct vec3 *source)
{
    dest->x -= source->x;
    dest->y -= source->y;
    dest->z -= source->z;
}

void vec3_mul(struct vec3 *dest, float source)
{
    dest->x *= source;
    dest->y *= source;
    dest->z *= source;
}

void vec3_div(struct vec3 *dest, float source)
{
    dest->x /= source;
    dest->y /= source;
    dest->z /= source;
}


void vec3_cross(struct vec3 *dest, struct vec3 const *v1, struct vec3 const *v2)
{
    dest->x = v1->y * v2->z - v1->z * v2->y;
    dest->y = v1->z * v2->x - v1->x * v2->z;
    dest->z = v1->x * v2->y - v1->y * v2->x;
}

float vec3_dot(struct vec3 const *v1, struct vec3 const *v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float vec3_length(struct vec3 const *vec)
{
    float a = vec3_dot(vec, vec);
    return sqrt(a);
}

void vec3_normalize(struct vec3 *vec)
{
    float len = vec3_length(vec);
    vec3_div(vec, len);
}

int hash_seed_from_floats(struct vec3 const *normal, struct vec3 const *direction)
{
    int x = (int)(normal->x * 73856093.0f);
    int y = (int)(normal->y * 19349663.0f);
    int z = (int)(normal->z * 83492791.0f);

    int dx = (int)(direction->x * 1640531513.0f);
    int dy = (int)(direction->y * 2654435761.0f);
    int dz = (int)(direction->z * 1013904223.0f);

    int seed = x ^ y ^ z ^ dx ^ dy ^ dz;
    return seed;
}

int vec3_near_zero(struct vec3 const *vec)
{
    float s = 1e-8f;

    return ((vec->x > 0 ? vec->x : - vec->x) < s && (vec->y > 0 ? vec->y : - vec->y) < s && (vec->z > 0 ? vec->z : - vec->z) < s);
}

void vec3_random_unit(struct vec3 *dest, int *rand_seed)
{
    float z = (rand01(*rand_seed) * 2.0f) - 1.0f;
    *rand_seed = randint(*rand_seed);

    float a = rand01(*rand_seed) * 2.0f * 3.14159265f;
    *rand_seed = randint(*rand_seed);

    float r = sqrt(fmax(0.0f, 1.0f - z * z));

    dest->x = r * cos(a);
    dest->y = r * sin(a);
    dest->z = z;
}

void vec3_reflect(struct vec3 *dest, struct vec3 const *v, struct vec3 const *n)
{
    float dot_product = vec3_dot(v, n);

    dest->x = v->x - 2 * dot_product * n->x;
    dest->y = v->y - 2 * dot_product * n->y;
    dest->z = v->z - 2 * dot_product * n->z;
}

void vec3_refract(struct vec3 *dest, struct vec3 const *uv, struct vec3 const *n, float etai_over_etat)
{
    struct vec3 min_uv = *uv;
    vec3_mul(&min_uv, -1);
    float cos_theta = fmin(vec3_dot(&min_uv, n), 1.0f);
    struct vec3 r_out_perp;

    r_out_perp.x = etai_over_etat * (uv->x + (cos_theta * n->x));
    r_out_perp.y = etai_over_etat * (uv->y + (cos_theta * n->y));
    r_out_perp.z = etai_over_etat * (uv->z + (cos_theta * n->z));

    struct vec3 r_out_parallel = *n;
    float aze = -1 * sqrt(fabs(1.0f - vec3_dot(&r_out_perp, &r_out_perp)));
    vec3_mul(&r_out_parallel, aze);

    dest->x = r_out_perp.x + r_out_parallel.x;
    dest->y = r_out_perp.y + r_out_parallel.y;
    dest->z = r_out_perp.z + r_out_parallel.z;
}
