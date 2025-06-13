#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <any>
#include <map>
#include <iostream>
#include "GPUDevice.hpp"

#ifndef GPUCONTEXT_HPP_
    #define GPUCONTEXT_HPP_

namespace gpu {
    class Context {
        public:
            Context();
            ~Context();

            bool init_noexcept(gpu::Device const &device) noexcept;
            void init(gpu::Device const &device);
            cl_context const &get_context(void) const;

            bool add_ressource(std::size_t id, void *object, std::size_t size, cl_mem_flags flags);

            void *get_common_space_ressources(std::size_t);
            std::map<std::size_t, cl_mem> &get_params(void);
        private:
            std::map<std::size_t, cl_mem> list_mem;
            cl_context context;
    };
}

#endif /* !GPUCONTEXT_HPP_ */
