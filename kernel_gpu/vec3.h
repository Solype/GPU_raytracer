

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef VEC3_H_
    #define VEC3_H_

struct vec3 {
    float x;
    float y;
    float z;
};

void vec3_add(struct vec3 *dest, const struct vec3 *source);
void vec3_sub(struct vec3 *dest, const struct vec3 *source);
void vec3_mul(struct vec3 *dest, const struct vec3 *source);
void vec3_div(struct vec3 *dest, const struct vec3 *source);
void vec3_normalize(struct vec3 *vec);
void vec3_cross(struct vec3 *dest, struct vec3 const *v1, struct vec3 const *v2);
float vec3_dot(struct vec3 const *v1, struct vec3 const *v2);
float vec3_length(struct vec3 const *vec);
int hash_seed_from_floats(struct vec3 const *normal, struct vec3 const *direction);
int vec3_near_zero(struct vec3 const *vec);
void vec3_reflect(struct vec3 *dest, struct vec3 const *v, struct vec3 const *n);
void vec3_random_unit(struct vec3 *dest, int *rand_seed);


#endif /* !VEC3_H_ */


#ifdef __cplusplus
    }
#endif
