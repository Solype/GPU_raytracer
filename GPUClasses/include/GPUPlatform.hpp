#ifndef GPUPLATFORM_HPP_
    #define GPUPLATFORM_HPP_

    #include <CL/cl.h>

namespace gpu {
    class Platform {
        public:
            Platform();
            ~Platform();
            cl_platform_id const &get_platform() const;
            bool load_noexcept(void) noexcept;
            void load(void);

        private:
            cl_platform_id platform;
    };
}

#endif /* !GPUPLATFORM_HPP_ */
