#include "GPUPlatform.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

gpu::Platform::Platform() :
    platform(nullptr)
{
}

gpu::Platform::~Platform()
{
}

bool gpu::Platform::load_noexcept(void) noexcept
{
    cl_uint platformCount = 0;
    cl_int err = clGetPlatformIDs(0, nullptr, &platformCount);

    if (err != CL_SUCCESS || platformCount == 0) {
        std::cerr << "Aucune plateforme OpenCL trouvée ! Code: " << err << std::endl;
        return false;
    }
    std::vector<cl_platform_id> platforms(platformCount);
    err = clGetPlatformIDs(platformCount, platforms.data(), nullptr);
    if (err != CL_SUCCESS) {
        std::cerr << "Error: clGetPlatformIDs failed. Code: " << err << std::endl;
        return false;
    }
    this->platform = platforms[0];
    return true;
}

void gpu::Platform::load(void)
{
    if (this->load_noexcept() == false) {
        throw std::runtime_error("[GPU] failed to load platform");
    }
}

cl_platform_id const &gpu::Platform::get_platform(void) const
{
    return this->platform;
}
