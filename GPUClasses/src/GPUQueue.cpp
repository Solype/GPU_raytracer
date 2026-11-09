#include "GPUQueue.hpp"
#include <iostream>
#include <stdexcept>

gpu::Queue::Queue() :
    queue(nullptr)
{
}

gpu::Queue::~Queue()
{
    if (this->queue != nullptr) {
        clReleaseCommandQueue(this->queue);
    }
}

bool gpu::Queue::init_noexcept(gpu::Context const &context, gpu::Device const &device) noexcept
{
    cl_int err;

    if (this->queue != nullptr) {
        clReleaseCommandQueue(this->queue);
    }
    this->queue = clCreateCommandQueueWithProperties(context.get_context(), device.get_device(), nullptr, &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Erreur clCreateCommandQueue: " << err << std::endl;
        this->queue = nullptr;
        return false;
    }
    return true;
}

void gpu::Queue::init(gpu::Context const &context, gpu::Device const &device)
{
    if (this->init_noexcept(context, device) == false) {
        throw std::runtime_error("[GPU] failed to create queue");
    }
}

cl_command_queue const &gpu::Queue::get_queue(void) const
{
    return this->queue;
}

bool gpu::Queue::run(gpu::Kernel const &kernel, std::size_t global_work_size)
{
    cl_int err = clEnqueueNDRangeKernel(this->queue, kernel.get_kernel(), 1, nullptr, &global_work_size, nullptr, 0, nullptr, nullptr);

    if (err == CL_SUCCESS) {
        err = clFinish(this->queue);
    }
    if (err != CL_SUCCESS) {
        std::cerr << "[GPU] kernel run failed: " << err << std::endl;
        return false;
    }
    return true;
}

bool gpu::Queue::read_result(gpu::Context const &context, std::size_t id, std::size_t size, void *buffer)
{
    cl_mem mem = context.get_resource(id);

    if (mem == nullptr) {
        std::cerr << "[GPU] no resource at id " << id << std::endl;
        return false;
    }
    cl_int err = clEnqueueReadBuffer(this->queue, mem, CL_TRUE, 0, size, buffer, 0, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        std::cerr << "[GPU] clEnqueueReadBuffer failed: " << err << std::endl;
        return false;
    }
    return true;
}
