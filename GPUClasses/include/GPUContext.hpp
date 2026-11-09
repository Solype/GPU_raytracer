#ifndef GPUCONTEXT_HPP_
    #define GPUCONTEXT_HPP_

    #include <CL/cl.h>
    #include <map>
    #include "GPUDevice.hpp"

namespace gpu {
    class Context {
        public:
            Context();
            ~Context();
            Context(Context const &) = delete;
            Context &operator=(Context const &) = delete;

            bool init_noexcept(gpu::Device const &device) noexcept;
            void init(gpu::Device const &device);
            cl_context const &get_context(void) const;

            // id is the kernel argument index; replaces any buffer already at that id
            bool add_resource(std::size_t id, void *object, std::size_t size, cl_mem_flags flags);
            cl_mem get_resource(std::size_t id) const;
            std::map<std::size_t, cl_mem> const &get_params(void) const;

        private:
            std::map<std::size_t, cl_mem> list_mem;
            cl_context context;
    };
}

#endif /* !GPUCONTEXT_HPP_ */
