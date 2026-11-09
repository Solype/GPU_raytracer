#ifndef PRIMITIVES_HPP_
    #define PRIMITIVES_HPP_

    #include "struct.h"

class IPrimitive {
    public:
        virtual ~IPrimitive() = default;
        virtual struct primitive to_struct() const = 0;
};

class APrimitive : public IPrimitive {
    public:
        struct primitive to_struct() const override;

    protected:
        struct primitive primitive;
};

class Sphere : public APrimitive {
    public:
        Sphere(float x, float y, float z, float radius, int material_id = -1);
};

#endif /* !PRIMITIVES_HPP_ */
