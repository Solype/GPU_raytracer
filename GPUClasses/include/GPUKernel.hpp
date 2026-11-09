#ifndef GPUKERNEL_HPP_
    #define GPUKERNEL_HPP_

    #include <CL/cl.h>
    #include <string>
    #include "GPUContext.hpp"
    #include "GPUProgram.hpp"

namespace gpu {
    class Kernel {
        public:
            Kernel();
            ~Kernel();
            Kernel(Kernel const &) = delete;
            Kernel &operator=(Kernel const &) = delete;

            cl_kernel const &get_kernel() const;
            void init(gpu::Program const &prog, std::string const &entry_point);
            bool init_noexcept(gpu::Program const &prog, std::string const &entry_point) noexcept;
            bool load_params(gpu::Context const &context);

        private:
            cl_kernel kernel;
    };
}

#endif /* !GPUKERNEL_HPP_ */
