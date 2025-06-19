#include "struct.h"
#include <functional>

#pragma once
#ifndef MATERIAL_HPP_
    #define MATERIAL_HPP_

bool operator==(const struct material &lhs, const struct material &rhs);
namespace std {
    template <>
    struct hash<struct vec3> {
        std::size_t operator()(const struct vec3 &v) const noexcept {
            return hash<float>()(v.x) ^ hash<float>()(v.y) ^ hash<float>()(v.z);
        }
    };

    template <>
    struct hash<struct material> {
        std::size_t operator()(const struct material& m) const noexcept {
            size_t h = std::hash<int>()(m.type);
            switch (m.type) {
                case DIFFUSE:
                    return h ^ hash<struct vec3>()(m.data.diffuse.albedo);
                case METAL:
                    return h ^ hash<struct vec3>()(m.data.metal.albedo);
                    break;
            }
            return h;
        }
    };
}


class IMaterial {
    public:
        ~IMaterial() = default;
        virtual struct material to_struct(void) const = 0;
};

class AMaterial : public IMaterial {
    struct material to_struct(void) const override;

    protected:
        struct material mat;
};

class Diffuse : public AMaterial {
    public :
        Diffuse(float red, float green, float blue);
};

class Metal : public AMaterial {
    public :
        Metal(float red, float green, float blue, float fuzz = 1.0f);
};

class Dielectric : public AMaterial {
    public :
        Dielectric(float refraction_index);
};

#endif /* !MATERIAL_HPP_ */
