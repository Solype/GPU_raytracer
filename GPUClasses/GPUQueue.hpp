#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <vector>

#include "GPUContext.hpp"
#include "GPUDevice.hpp"
#include "GPUKernel.hpp"

#ifndef GPUQUEUE_HPP_
    #define GPUQUEUE_HPP_

namespace gpu {
    class Queue {
        public:
            Queue();
            ~Queue();

            bool init_noexcep(gpu::Context const &context, gpu::Device const &device) noexcept;
            void init(gpu::Context const &context, gpu::Device const &device);

            cl_command_queue const &get_queue(void) const;

            void run(gpu::Kernel const &kernel, std::size_t global_work_size);
            void read_result(gpu::Context &context, std::size_t id, std::size_t size, void *buffer);

        private:
            std::vector<cl_command_queue_properties> props;
            cl_command_queue queue;
    };
}

#endif /* !GPUQUEUE_HPP_ */
