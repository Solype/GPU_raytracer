#ifdef __cmakelist
    #include "sphere.h"
#else
    #include "kernel_gpu/sphere.h"
#endif

int sphere_hits(struct sphere const *sphere, struct ray const *ray, struct hits *rec, struct interval *interval)
{
    struct vec3 oc;
    struct vec3 normal;
    float oc_len;
    float ray_dir_len;

    oc.x = sphere->origin.x - ray->origin.x;
    oc.y = sphere->origin.y - ray->origin.y;
    oc.z = sphere->origin.z - ray->origin.z;
    oc_len = vec3_length(&oc);
    ray_dir_len = vec3_length(&ray->direction);

    float a = ray_dir_len * ray_dir_len;
    float h = vec3_dot(&oc, &ray->direction);
    float c = (oc_len * oc_len) - (sphere->radius * sphere->radius);

    float discriminant = h * h - a * c;
    if (discriminant < 0) {
        return 0;
    }
    

    float sqrtd = sqrt(discriminant);
    float root = (h - sqrtd) / a;
    if (!interval_surround(*interval, root)) {
        root = (h + sqrtd) / a;
        if (!interval_surround(*interval, root)) {
            return 0;
        }
    }
    rec->t = root;
    rec->point.x = ray->origin.x + rec->t * ray->direction.x;
    rec->point.y = ray->origin.y + rec->t * ray->direction.y;
    rec->point.z = ray->origin.z + rec->t * ray->direction.z;

    normal.x = rec->point.x - sphere->origin.x;
    normal.y = rec->point.y - sphere->origin.y;
    normal.z = rec->point.z - sphere->origin.z;
    hits_set_face_normal(rec, ray, &normal);
    vec3_normalize(&rec->normal);
    return 1;
}
