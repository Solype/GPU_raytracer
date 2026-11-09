#ifndef MATERIALS_HPP_
    #define MATERIALS_HPP_

    #include <functional>
    #include "struct.h"

bool operator==(const struct vec3 &lhs, const struct vec3 &rhs);
bool operator==(const struct material &lhs, const struct material &rhs);

namespace std {
    template <>
    struct hash<struct vec3> {
        std::size_t operator()(const struct vec3 &v) const noexcept {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1) ^ (hash<float>()(v.z) << 2);
        }
    };

    // must stay consistent with operator==: hash every field it compares
    template <>
    struct hash<struct material> {
        std::size_t operator()(const struct material &m) const noexcept {
            std::size_t h = std::hash<int>()(m.type);
            switch (m.type) {
                case DIFFUSE:
                    return h ^ (hash<struct vec3>()(m.data.diffuse.albedo) << 1);
                case METAL:
                    return h ^ (hash<struct vec3>()(m.data.metal.albedo) << 1) ^ (hash<float>()(m.data.metal.fuzz) << 2);
                case DIELECTRIC:
                    return h ^ (hash<float>()(m.data.dielectric.refraction_index) << 1);
            }
            return h;
        }
    };
}

class IMaterial {
    public:
        virtual ~IMaterial() = default;
        virtual struct material to_struct(void) const = 0;
};

class AMaterial : public IMaterial {
    public:
        struct material to_struct(void) const override;

    protected:
        struct material mat;
};

class Diffuse : public AMaterial {
    public:
        Diffuse(float red, float green, float blue);
};

class Metal : public AMaterial {
    public:
        Metal(float red, float green, float blue, float fuzz = 1.0f);
};

class Dielectric : public AMaterial {
    public:
        Dielectric(float refraction_index);
};

#endif /* !MATERIALS_HPP_ */
