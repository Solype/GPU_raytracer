

#ifdef __cmakelist
    #include "hits.h"
    #include "ray.h"
#else
    #include "kernel_gpu/hits.h"
    #include "kernel_gpu/ray.h"
#endif

void hits_set_face_normal(struct hits *hits, struct ray const *ray, struct vec3 const *outward_normal)
{
    hits->front_face = vec3_dot(&ray->direction, outward_normal) < 0;

    if (!((struct hits *)hits)->front_face) {
        hits->normal.x = -(outward_normal->x);
        hits->normal.y = -(outward_normal->y);
        hits->normal.z = -(outward_normal->z);
    } else {
        hits->normal.x = outward_normal->x;
        hits->normal.y = outward_normal->y;
        hits->normal.z = outward_normal->z;
    }
}