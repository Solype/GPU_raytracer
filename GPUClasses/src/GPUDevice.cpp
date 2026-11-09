#include "GPUDevice.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

gpu::Device::Device() :
    device(nullptr)
{
}

gpu::Device::~Device()
{
}

cl_device_id const &gpu::Device::get_device(void) const
{
    return device;
}

bool gpu::Device::load_noexcept(Platform const &platform) noexcept
{
    cl_uint deviceCount = 0;
    cl_int err;

    err = clGetDeviceIDs(platform.get_platform(), CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
    if (err != CL_SUCCESS || deviceCount == 0) {
        std::cerr << "Error: clGetDeviceIDs failed to get device count. Code: " << err << std::endl;
        return false;
    }
    std::vector<cl_device_id> devices(deviceCount);
    err = clGetDeviceIDs(platform.get_platform(), CL_DEVICE_TYPE_ALL, deviceCount, devices.data(), nullptr);
    if (err != CL_SUCCESS) {
        std::cerr << "Error: clGetDeviceIDs failed to get device IDs. Code: " << err << std::endl;
        return false;
    }
    this->device = devices[0];
    return true;
}

void gpu::Device::load(Platform const &platform)
{
    if (this->load_noexcept(platform) == false) {
        throw std::runtime_error("[GPU] failed to load device");
    }
}
