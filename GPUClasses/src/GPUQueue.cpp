#include "GPUQueue.hpp"
#include "GPUContext.hpp"
#include "GPUDevice.hpp"
#include <iostream>

gpu::Queue::Queue():
    props({ 0 }), queue(nullptr)
{
}

gpu::Queue::~Queue()
{
    if (this->queue != nullptr) {
        clReleaseCommandQueue(this->queue);
    }
}

bool gpu::Queue::init_noexcep(gpu::Context const &context, gpu::Device const &device) noexcept
{
    cl_int err;

    this->queue = clCreateCommandQueueWithProperties(context.get_context(), device.get_device(), props.data(), &err);
    if (err != CL_SUCCESS) {
        std::cerr << "Erreur clCreateCommandQueue: " << err << std::endl;
        return false;
    }
    return true;
}

void gpu::Queue::init(gpu::Context const &context, gpu::Device const &device)
{
    if (this->init_noexcep(context, device) == false) {
        throw std::runtime_error("[GPU] failed to create queue");
    }
}

cl_command_queue const &gpu::Queue::get_queue(void) const
{
    return this->queue;
}

void gpu::Queue::run(gpu::Kernel const &kernel, std::size_t global_work_size)
{
    cl_int err;

    err = clEnqueueNDRangeKernel(this->queue, kernel.get_kernel(), 1, nullptr, &global_work_size, nullptr, 0, nullptr, nullptr);
    clFlush(this->queue);
    clFinish(this->queue);
}

void gpu::Queue::read_result(gpu::Context &context, std::size_t id, std::size_t size, void *buffer)
{
    clEnqueueReadBuffer(this->queue, context.get_params()[id], CL_TRUE, 0, size, buffer, 0, nullptr, nullptr);
}
