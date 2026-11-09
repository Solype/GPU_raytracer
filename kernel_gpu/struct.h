#ifndef STRUCT_H_
    #define STRUCT_H_

    #include "sphere.h"
    #include "camera.h"
    #include "materials.h"

enum primitive_e {
    SPHERE = 1,
};

enum material_e {
    DIFFUSE = 1,
    METAL = 2,
    DIELECTRIC = 3,
};

struct material {
    enum material_e type;
    union {
        struct material_diffuse diffuse;
        struct material_metal metal;
        struct material_dielectric dielectric;
    } data;
};

struct primitive {
    enum primitive_e type;
    unsigned int mat_id;
    union {
        struct sphere sphere;
    } data;
};

struct scene_data {
    unsigned int rand_seed;
    unsigned int nb_materials;
    unsigned int nb_primitives;
    struct camera cam;
};

#endif /* !STRUCT_H_ */
