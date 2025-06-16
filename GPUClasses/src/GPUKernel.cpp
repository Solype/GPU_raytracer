#include "GPUKernel.hpp"
#include <iostream>

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
    this->kernel = clCreateKernel(prog.get_program(), entry_point.data(), &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Failed to create kernel " << err << std::endl;
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

bool gpu::Kernel::load_params(gpu::Context &context)
{
    for (auto [ id, mem ] : context.get_params()) {
        clSetKernelArg(this->kernel, id, sizeof(cl_mem), &mem);
    }
    return true;
}
