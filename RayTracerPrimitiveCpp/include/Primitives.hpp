#include "struct.h"

#ifndef IPRIMITIVE_HPP_
    #define IPRIMITIVE_HPP_


class IPrimitive {
    public:
        ~IPrimitive() = default;
        virtual struct primitive to_struct() const = 0;
};

class APrimitive : public IPrimitive {
    public :
        struct primitive to_struct() const override;
    protected:
        struct primitive primitive;
};

class Sphere : public APrimitive {
    public :
        Sphere(float x, float y, float z, float radius, int material_id = -1);
};

#endif /* !IPRIMITIVE_HPP_ */
