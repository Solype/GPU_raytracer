#include "RayTracerRegistry.hpp"
#include <stdexcept>

RayTracerRegistry::RayTracerRegistry()
{
}

RayTracerRegistry::~RayTracerRegistry()
{
}

int RayTracerRegistry::register_primitive(IPrimitive const &primitive, IMaterial const &material)
{
    auto mat_s = material.to_struct();
    auto it = this->material_registry.find(mat_s);
    auto primi_s = primitive.to_struct();

    if (it == this->material_registry.end()) {
        primi_s.mat_id = this->materials.size();
        this->material_registry.emplace(mat_s, primi_s.mat_id);
        this->materials.emplace_back(mat_s);
    } else {
        primi_s.mat_id = it->second;
    }
    this->primitives.emplace_back(primi_s);
    return primi_s.mat_id;
}

void RayTracerRegistry::register_primitive(IPrimitive const &primitive, int const &material)
{
    if (material < 0) {
        throw std::runtime_error("Mauvais id de materiaux !");
    }
    auto primi_s = primitive.to_struct();
    primi_s.mat_id = material;
    this->primitives.emplace_back(primi_s);
}

void RayTracerRegistry::register_primitive(IPrimitive const &primitive)
{
    this->primitives.emplace_back(primitive.to_struct());
}

int RayTracerRegistry::get_mat_id(IMaterial const &material) const
{
    auto mat_s = material.to_struct();
    auto it = this->material_registry.find(mat_s);

    if (it == this->material_registry.end()) {
        return -1;
    }
    return it->second;
}

std::vector<struct material> const &RayTracerRegistry::get_materials(void) const
{
    return this->materials;
}

std::vector<struct primitive> const &RayTracerRegistry::get_primitives(void) const
{
    return this->primitives;
}

std::vector<struct material> &RayTracerRegistry::get_materials(void)
{
    return this->materials;
}

std::vector<struct primitive> &RayTracerRegistry::get_primitives(void)
{
    return this->primitives;
}
