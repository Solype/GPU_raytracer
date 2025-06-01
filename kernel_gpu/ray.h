#ifdef __cmakelist
    #include "vec3.h"
#else
    #include "kernel_gpu/vec3.h"
#endif

#ifndef RAY_H_
    #define RAY_H_


struct ray {
    struct vec3 origin;
    struct vec3 direction;
};

#endif /* !RAY_H_ */
