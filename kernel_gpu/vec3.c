#include "vec3.h"

#ifdef __cmakelist
    #include <math.h>
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

void vec3_mul(struct vec3 *dest, const struct vec3 *source)
{
    dest->x *= source->x;
    dest->y *= source->y;
    dest->z *= source->z;
}

void vec3_div(struct vec3 *dest, const struct vec3 *source)
{
    dest->x /= source->x;
    dest->y /= source->y;
    dest->z /= source->z;
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

    vec->x /= len;
    vec->y /= len;
    vec->z /= len;
}
