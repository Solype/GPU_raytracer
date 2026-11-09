#ifndef GPUDEVICE_HPP_
    #define GPUDEVICE_HPP_

    #include <CL/cl.h>
    #include "GPUPlatform.hpp"

namespace gpu {
    class Device {
        public:
            Device();
            ~Device();

            cl_device_id const &get_device() const;
            bool load_noexcept(Platform const &platform) noexcept;
            void load(Platform const &platform);

        private:
            cl_device_id device;
    };
}

#endif /* !GPUDEVICE_HPP_ */
