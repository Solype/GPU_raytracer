
#ifdef __cmakelist
    #include "vec3.h"
    #include "ray.h"
    #include "hits.h"
    #include "interval.h"
#else
    #include "kernel_gpu/vec3.h"
    #include "kernel_gpu/ray.h"
    #include "kernel_gpu/hits.h"
    #include "kernel_gpu/interval.h"
#endif

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef SCHERE_H_
    #define SCHERE_H_

struct sphere {
    struct vec3 origin;
    float radius;
};

int sphere_hits(struct sphere const *sphere, struct ray const *ray, struct hits *rec, struct interval *interval);

#endif /* !SCHERE_H_ */

#ifdef __cplusplus
    }
#endif