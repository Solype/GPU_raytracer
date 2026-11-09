#include "GPUKernel.hpp"
#include <iostream>
#include <stdexcept>

gpu::Kernel::Kernel() :
    kernel(nullptr)
{
}

gpu::Kernel::~Kernel()
{
    if (this->kernel != nullptr) {
        clReleaseKernel(this->kernel);
    }
}

cl_kernel const &gpu::Kernel::get_kernel(void) const
{
    return this->kernel;
}

bool gpu::Kernel::init_noexcept(gpu::Program const &prog, std::string const &entry_point) noexcept
{
    cl_int err;

    if (this->kernel != nullptr) {
        clReleaseKernel(this->kernel);
    }
    this->kernel = clCreateKernel(prog.get_program(), entry_point.c_str(), &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Failed to create kernel " << err << std::endl;
        this->kernel = nullptr;
        return false;
    }
    return true;
}

void gpu::Kernel::init(gpu::Program const &prog, std::string const &entry_point)
{
    if (this->init_noexcept(prog, entry_point) == false) {
        throw std::runtime_error("[GPU] Failed to create kernel");
    }
}

bool gpu::Kernel::load_params(gpu::Context const &context)
{
    for (auto const &[id, mem] : context.get_params()) {
        if (this->set_arg(id, sizeof(cl_mem), &mem) == false) {
            return false;
        }
    }
    return true;
}

bool gpu::Kernel::set_arg(std::size_t id, std::size_t size, void const *value)
{
    cl_int err = clSetKernelArg(this->kernel, id, size, value);

    if (err != CL_SUCCESS) {
        std::cerr << "[GPU] clSetKernelArg failed for argument " << id << ": " << err << std::endl;
        return false;
    }
    return true;
}
