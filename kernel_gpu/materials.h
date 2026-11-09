#ifndef MATERIALS_H_
    #define MATERIALS_H_

    #include "vec3.h"

struct material_diffuse {
    struct vec3 albedo;
};

struct material_metal {
    struct vec3 albedo;
    float fuzz;
};

struct material_dielectric {
    float refraction_index;
};

#endif /* !MATERIALS_H_ */
