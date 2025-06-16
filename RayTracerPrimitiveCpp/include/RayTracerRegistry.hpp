#include "struct.h"
#include "Primitives.hpp"
#include "Materials.hpp"
#include <vector>
#include <unordered_map>

#ifndef RAYTRACERREGISTRY_HPP_
    #define RAYTRACERREGISTRY_HPP_

class RayTracerRegistry {
    public:
        RayTracerRegistry();
        ~RayTracerRegistry();

        int register_primitive(IPrimitive const &primitive, IMaterial const &material);
        void register_primitive(IPrimitive const &primitive, int const &material);
        void register_primitive(IPrimitive const &primitive);
        int get_mat_id(IMaterial const &material) const;
        std::vector<struct material> const &get_materials(void) const;
        std::vector<struct primitive> const &get_primitives(void) const;
        std::vector<struct primitive> &get_primitives(void);
        std::vector<struct material> &get_materials(void);


    protected:
        std::vector<struct primitive> primitives;
        std::vector<struct material> materials;
        std::unordered_map<struct material, int> material_registry;
};

#endif /* !RAYTRACERREGISTRY_HPP_ */
