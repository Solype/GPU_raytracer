#ifndef GPUPROGRAM_HPP_
    #define GPUPROGRAM_HPP_

    #include <CL/cl.h>
    #include <list>
    #include <string>
    #include "GPUContext.hpp"
    #include "GPUDevice.hpp"

namespace gpu {
    class Program {
        public:
            Program();
            ~Program();
            Program(Program const &) = delete;
            Program &operator=(Program const &) = delete;

            int add_module(std::string const &path);
            // options are passed to clBuildProgram, e.g. "-I kernel_gpu"
            void compile(gpu::Context const &context, gpu::Device const &device, std::string const &options = "");
            bool compile_noexcept(gpu::Context const &context, gpu::Device const &device, std::string const &options = "") noexcept;
            cl_program const &get_program(void) const;

        private:
            cl_program program;
            std::list<std::string> _modules_to_load;
    };
}

#endif /* !GPUPROGRAM_HPP_ */
