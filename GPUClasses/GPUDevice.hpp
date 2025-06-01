#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include "GPUPlateform.hpp"

#ifndef GPUDEVICE_HPP_
    #define GPUDEVICE_HPP_

namespace gpu {
    class Device {
        public:
            Device();
            ~Device();

            cl_device_id const &get_device() const;
            bool load_noexcept(Plateform const &plateform) noexcept;
            void load(Plateform const &plateform);

        private:
            cl_device_id device;
    };
}

#endif /* !GPUDEVICE_HPP_ */
