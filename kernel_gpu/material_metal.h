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

#ifndef MATERIAL_METAL_H_
    #define MATERIAL_METAL_H_

struct material_metal {
    struct vec3 albedo;
};
    
int material_metal(struct material_metal const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation, struct ray *scattered);

#endif /* !MATERIAL_METAL_H_ */
