#include "hits.h"

// normal always points against the ray, front_face tells which side was hit
void hits_set_face_normal(struct hits *rec, float3 ray_direction, float3 outward_normal)
{
    rec->front_face = dot(ray_direction, outward_normal) < 0.0f;
    rec->normal = rec->front_face ? outward_normal : -outward_normal;
}
