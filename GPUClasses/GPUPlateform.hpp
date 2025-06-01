#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#ifndef GPUPLATEFORM_HPP_
    #define GPUPLATEFORM_HPP_

namespace gpu {
    class Plateform {
        public:
            Plateform();
            ~Plateform();
            cl_platform_id const &get_plateform() const;
            bool load_noexcept(void) noexcept;
            void load(void);

        private:
            cl_platform_id platform;
    };
}

#endif /* !GPUPLATEFORM_HPP_ */
