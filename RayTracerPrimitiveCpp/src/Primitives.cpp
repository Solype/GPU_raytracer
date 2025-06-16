#include "Primitives.hpp"
#include "struct.h"

struct primitive APrimitive::to_struct(void) const
{
    return this->primitive;
}

Sphere::Sphere(float x, float y, float z, float radius, int material_id)
{
    this->primitive.mat_id = material_id;
    this->primitive.type = SHPERE;
    this->primitive.data.sphere = (struct sphere) {
        .origin = (struct vec3){.x = x, .y = y, .z = z},
        .radius = radius
    };
}
