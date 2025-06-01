#define CL_TARGET_OPENCL_VERSION 220
#include "GPUPlateform.hpp"
#include <CL/cl.h>
#include <vector>
#include <iostream>

gpu::Plateform::Plateform() :
    platform(nullptr)
{
}

gpu::Plateform::~Plateform()
{
}

bool gpu::Plateform::load_noexcept(void) noexcept
{
    cl_uint platformCount;

    clGetPlatformIDs(0, nullptr, &platformCount);
    if (platformCount == 0) {
        std::cerr << "Aucune plateforme OpenCL trouvée !" << std::endl;
        return false;
    }
    std::vector<cl_platform_id> platforms(platformCount);
    clGetPlatformIDs(platformCount, platforms.data(), nullptr);

    this->platform = platforms[0];
    return true;
}

void gpu::Plateform::load(void)
{
    cl_uint platformCount;

    clGetPlatformIDs(0, nullptr, &platformCount);
    if (platformCount == 0) {
        std::cerr << "Aucune plateforme OpenCL trouvée !" << std::endl;
        throw std::runtime_error("gpu::Plateform constructor failed due");
    }
    std::vector<cl_platform_id> platforms(platformCount);
    clGetPlatformIDs(platformCount, platforms.data(), nullptr);
    this->platform = platforms[0];
}

cl_platform_id const &gpu::Plateform::get_plateform(void) const
{
    return this->platform;
}
