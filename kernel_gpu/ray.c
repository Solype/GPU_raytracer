#ifdef __cmakelist
    #include "ray.h"
#else
    #include "kernel_gpu/ray.h"
#endif

void ray_at(struct vec3 *dest, struct ray const *ray, float const t)
{
    dest->x = ray->origin.x + t * ray->direction.x;
    dest->y = ray->origin.y + t * ray->direction.y;
    dest->z = ray->origin.z + t * ray->direction.z;
}
