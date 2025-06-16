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

#ifndef MATERIAL_DIFFUSE_H_
    #define MATERIAL_DIFFUSE_H_

struct material_diffuse {
    struct vec3 albedo;
};

int material_diffuse(struct material_diffuse const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation, struct ray *scattered);

#endif /* !MATERIAL_DIFFUSE_H_ */
