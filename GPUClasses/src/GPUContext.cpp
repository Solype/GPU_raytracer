#include "GPUContext.hpp"
#include <iostream>
#include <stdexcept>

gpu::Context::Context() :
    context(nullptr)
{}

gpu::Context::~Context()
{
    // buffers belong to the context: release them first
    for (auto const &[id, mem] : this->list_mem) {
        if (mem != nullptr) {
            clReleaseMemObject(mem);
        }
    }
    if (context != nullptr) {
        clReleaseContext(context);
    }
}

bool gpu::Context::init_noexcept(gpu::Device const &device) noexcept
{
    cl_int err;

    if (this->context != nullptr) {
        clReleaseContext(this->context);
    }
    this->context = clCreateContext(nullptr, 1, &device.get_device(), nullptr, nullptr, &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Erreur clCreateContext: " << err << std::endl;
        this->context = nullptr;
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

std::map<std::size_t, cl_mem> const &gpu::Context::get_params(void) const
{
    return this->list_mem;
}

bool gpu::Context::add_resource(std::size_t id, void *object, std::size_t size, cl_mem_flags flags)
{
    cl_mem mem = nullptr;
    cl_int err;

    // size 0 keeps a null buffer: valid as a kernel argument
    if (size != 0) {
        mem = clCreateBuffer(this->context, flags, size, object, &err);
        if (err != CL_SUCCESS) {
            std::cerr << "[GPU] Error while allocating resource " << id << ": " << err << std::endl;
            return false;
        }
    }
    auto it = this->list_mem.find(id);
    if (it != this->list_mem.end() && it->second != nullptr) {
        clReleaseMemObject(it->second);
    }
    this->list_mem[id] = mem;
    return true;
}

cl_mem gpu::Context::get_resource(std::size_t id) const
{
    auto it = this->list_mem.find(id);

    return it != this->list_mem.end() ? it->second : nullptr;
}
