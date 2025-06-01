
#ifdef __cmakelist
    #include "interval.h"
    #include "ray.h"
    #include "hits.h"
    // extern "C" {
#else
    #include "kernel_gpu/interval.h"
    #include "kernel_gpu/ray.h"
    #include "kernel_gpu/hits.h"
#endif

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef CAMERA_H_
    #define CAMERA_H_

struct camera {
    float focal_length;
    float view_port_height;
    float view_port_width;
    int image_height;
    int image_width;
    int sample_pixel;

    struct vec3 center;
    struct vec3 view_port_u;
    struct vec3 view_port_v;
    struct vec3 pixel_delta_u;
    struct vec3 pixel_delta_v;
    struct vec3 view_port_upperleft;
    struct vec3 pixel00_loc;
};

void camera_compute_ray(struct ray *r, struct camera const *cam, int i, int j, int k);
void create_camera(struct camera *cam, int width, int height, float focal);
// void ray_color(struct ray *r);
void color_hit(struct vec3 *color, struct hits const *ray);

#endif /* !CAMERA_H_ */

#ifdef __cplusplus
    }
#endif
#ifdef __cmakelist
// }
#endif
