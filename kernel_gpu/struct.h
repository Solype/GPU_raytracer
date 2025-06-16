#include "sphere.h"
#include "camera.h"

#ifndef STRUCT_H_
    #define STRUCT_H_

enum primitive_e {
    SHPERE = 1,
};
enum material_e {
    DIFFUSE = 1,
};
enum light_e {
    LIGHT_POINT = 1,
    DIRECTION_LIGHT = 2
};


struct material {
    enum material_e type;
    union {
    } data;
};

struct primitive {
    enum primitive_e type;
    unsigned int mat_id;
    union {
        struct sphere sphere;
    } data;
};

struct light {
    enum light_e type;
    union {
        struct vec3 point;
    } data;
};

struct scene_data {
    int rand_seed;
    unsigned int nb_lights;
    unsigned int nb_materials;
    unsigned int nb_primitives;
    struct camera cam;
};

#endif /* !STRUCT_H_ */
