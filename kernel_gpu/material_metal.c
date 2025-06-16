#ifdef __cmakelist
    #include "vec3.h"
    #include "ray.h"
    #include "hits.h"
    #include "interval.h"
    #include "material_metal.h"
#else
    #include "kernel_gpu/vec3.h"
    #include "kernel_gpu/ray.h"
    #include "kernel_gpu/hits.h"
    #include "kernel_gpu/interval.h"
    #include "kernel_gpu/material_metal.h"
#endif

int material_metal(struct material_metal const *mat, const struct ray *r_in, const struct hits *rec, struct vec3 *attenuation, struct ray *scattered)
{

    vec3_reflect(&scattered->direction, &r_in->direction, &rec->normal);
    scattered->origin = rec->point;
    *attenuation = mat->albedo;
}
