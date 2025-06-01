#include "sphere.h"

#ifndef STRUCT_H_
    #define STRUCT_H_

enum primitive_e {
    SHPERE = 1,
};

struct primitive {
    enum primitive_e type;
    union {
        struct sphere sphere;
    } data;
};

enum light_e {
    LIGHT_POINT = 1,
    DIRECTION_LIGHT = 2
};

struct light {
    enum light_e type;
    union {
        struct vec3 point;
    } data;
};

#endif /* !STRUCT_H_ */
