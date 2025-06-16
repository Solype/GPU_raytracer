#ifdef __cmakelist
    #include "vec3.h"
    #include "ray.h"
#else
    #include "kernel_gpu/vec3.h"
    #include "kernel_gpu/ray.h"
#endif



#ifndef HITS_H_
    #define HITS_H_

struct hits {
    float  t;
    struct vec3 normal;
    struct vec3 point;
    int   front_face;
    int mat_id;
};

void hits_set_face_normal(struct hits *hits, struct ray const *ray, struct vec3 const *outward_normal);

#endif /* !HITS_H_ */
