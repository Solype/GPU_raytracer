#ifndef CAMERA_H_
    #define CAMERA_H_

    #include "vec3.h"

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

#ifdef __cplusplus
extern "C" {
#endif

// host only: fills the viewport geometry, sample_pixel is left to the caller
void create_camera(struct camera *cam, int width, int height, float focal);

#ifdef __cplusplus
}
#endif

#endif /* !CAMERA_H_ */
