#ifndef GPUQUEUE_HPP_
    #define GPUQUEUE_HPP_

    #include <CL/cl.h>
    #include "GPUContext.hpp"
    #include "GPUDevice.hpp"
    #include "GPUKernel.hpp"

namespace gpu {
    class Queue {
        public:
            Queue();
            ~Queue();
            Queue(Queue const &) = delete;
            Queue &operator=(Queue const &) = delete;

            bool init_noexcept(gpu::Context const &context, gpu::Device const &device) noexcept;
            void init(gpu::Context const &context, gpu::Device const &device);

            cl_command_queue const &get_queue(void) const;

            // both block until the GPU is done
            bool run(gpu::Kernel const &kernel, std::size_t global_work_size);
            bool read_result(gpu::Context const &context, std::size_t id, std::size_t size, void *buffer);

        private:
            cl_command_queue queue;
    };
}

#endif /* !GPUQUEUE_HPP_ */
