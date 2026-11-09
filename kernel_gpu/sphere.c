#include "sphere.h"
#include "ray.h"
#include "hits.h"
#include "interval.h"

// writes rec only on a hit; mat_id is left to the caller
int sphere_hits(struct sphere const *sphere, struct ray const *ray, struct interval const *interval, struct hits *rec)
{
    float3 center = vec3_load(sphere->origin);
    float3 oc = center - ray->origin;
    float a = dot(ray->direction, ray->direction);
    float h = dot(ray->direction, oc);
    float c = dot(oc, oc) - sphere->radius * sphere->radius;
    float discriminant = h * h - a * c;

    if (discriminant < 0.0f) {
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
    rec->point = ray->origin + root * ray->direction;
    hits_set_face_normal(rec, ray->direction, (rec->point - center) / sphere->radius);
    return 1;
}
