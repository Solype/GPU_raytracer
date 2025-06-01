#include "GPUDevice.hpp"
#include <vector>
#include <iostream>

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

bool gpu::Device::load_noexcept(Plateform const &plateform) noexcept
{
    cl_uint deviceCount = 0;
    cl_int err;

    err = clGetDeviceIDs(plateform.get_plateform(), CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
    if (err != CL_SUCCESS || deviceCount == 0) {
        std::cerr << "Error: clGetDeviceIDs failed to get device count. Code: " << err << std::endl;
        return false;
    }
    std::vector<cl_device_id> devices(deviceCount);
    err = clGetDeviceIDs(plateform.get_plateform(), CL_DEVICE_TYPE_ALL, deviceCount, devices.data(), nullptr);
    if (err != CL_SUCCESS) {
        std::cerr << "Error: clGetDeviceIDs failed to get device IDs. Code: " << err << std::endl;
        return false;
    }
    this->device = devices[0];
    return true;
}

void gpu::Device::load(Plateform const &plateform)
{
    cl_uint deviceCount = 0;
    cl_int err;

    err = clGetDeviceIDs(plateform.get_plateform(), CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
    if (err != CL_SUCCESS || deviceCount == 0) {
        throw std::runtime_error("clGetDeviceIDs failed to get device count. Code: " + std::to_string(err));
    }
    std::vector<cl_device_id> devices(deviceCount);
    err = clGetDeviceIDs(plateform.get_plateform(), CL_DEVICE_TYPE_ALL, deviceCount, devices.data(), nullptr);
    if (err != CL_SUCCESS) {
        throw std::runtime_error("clGetDeviceIDs failed to get device IDs. Code: " + std::to_string(err));
    }

    this->device = devices[0];
}
