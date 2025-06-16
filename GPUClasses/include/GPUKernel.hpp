#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include "GPUContext.hpp"
#include "GPUProgram.hpp"

#ifndef GPUKERNEL_HPP_
    #define GPUKERNEL_HPP_

namespace gpu {
    class Kernel {
        public:
            Kernel();
            ~Kernel();

            cl_kernel const &get_kernel() const;
            void init(gpu::Program const &prog, std::string const &entry_point);
            bool init_noexcept(gpu::Program const &prog, std::string const &entry_point) noexcept;
            bool load_params(gpu::Context &context);

        private:
            cl_kernel kernel;
    };
}

#endif /* !GPUKERNEL_HPP_ */
