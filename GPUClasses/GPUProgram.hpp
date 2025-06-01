#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <string>
#include <list>
#include "GPUContext.hpp"

#ifndef GPULOADER_HPP_
    #define GPULOADER_HPP_


namespace gpu {
    class Program {
        public:
            Program();
            ~Program();

            int add_module(std::string const &path);
            void compile(gpu::Context const &context, gpu::Device const &device);
            bool compile_noexcept(gpu::Context const &context, gpu::Device const &device) noexcept;
            cl_program const &get_program(void) const;

        private:
            cl_program program;
            std::list<std::string> _modules_to_load;
    };
}

#endif /* !GPULOADER_HPP_ */
