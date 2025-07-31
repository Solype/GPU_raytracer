#include "GPUContext.hpp"
#include "GPUKernel.hpp"
#include <iostream>

gpu::Context::Context() :
    context(nullptr)
{}

gpu::Context::~Context()
{
    if (context != nullptr) {
        clReleaseContext(context);
    }
    for (auto ele : this->list_mem) {
        clReleaseMemObject(ele.second);
    }
}

bool gpu::Context::init_noexcept(gpu::Device const &device) noexcept
{
    cl_int err;

    this->context = clCreateContext(nullptr, 1, &device.get_device(), nullptr, nullptr, &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Erreur clCreateContext: " << err << std::endl;
        return false;
    }
    return true;
}

void gpu::Context::init(gpu::Device const &device)
{
    if (init_noexcept(device) == false) {
        throw std::runtime_error("[GPU] fail of context init");
    }
}

cl_context const &gpu::Context::get_context(void) const
{
    return this->context;
}

std::map<std::size_t, cl_mem> &gpu::Context::get_params(void)
{
    return this->list_mem;
}

bool gpu::Context::add_ressource(std::size_t id, void *object, std::size_t size, cl_mem_flags flags)
{
    cl_mem mem;
    cl_int err;
    auto obj_at_id = this->list_mem.find(id);

    if (obj_at_id != this->list_mem.end() && obj_at_id->second != nullptr) {
        clReleaseMemObject(obj_at_id->second);
    }
    if (size == 0) {
        std::cerr << "Attention, allocation null" << std::endl;
        this->list_mem.emplace(id, nullptr);
        return true;
    }
    mem = clCreateBuffer(this->context, flags, size, object, &err);
    if (err != CL_SUCCESS) {
        clReleaseMemObject(mem);
        std::cerr << "[GPU] Error while allocating ressources: " << err << std::endl;
        return false;
    }
    this->list_mem.emplace(id, mem);
    return true;
}

void *gpu::Context::get_common_space_ressources(std::size_t id)
{
    auto it = this->list_mem.find(id);

    if (it != list_mem.end()) {
        return it->second;
    }
    return nullptr;
}
