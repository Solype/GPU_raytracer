#include "struct.h"
#include "Materials.hpp"
#include <functional>

bool operator==(const struct vec3 &lhs, const struct vec3 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator==(const struct material &lhs, const struct material &rhs)
{
    if (lhs.type != rhs.type)
        return false;

    switch (lhs.type) {
        case DIFFUSE:
            return lhs.data.diffuse.albedo == rhs.data.diffuse.albedo;
        case METAL:
            return lhs.data.metal.albedo == rhs.data.metal.albedo;
        default:
            return false;
    }
}

struct material AMaterial::to_struct() const
{
    return this->mat;
}

Diffuse::Diffuse(float red, float green, float blue)
{
    this->mat.type = DIFFUSE;
    this->mat.data.diffuse.albedo = (struct vec3){.x = red, .y = green, .z = blue};
}

Metal::Metal(float red, float green, float blue, float fuzz)
{
    this->mat.type = METAL;
    this->mat.data.metal.albedo = (struct vec3){.x = red, .y = green, .z = blue};
    this->mat.data.metal.fuzz = std::min(std::max(0.0f, fuzz), 1.0f);
}
