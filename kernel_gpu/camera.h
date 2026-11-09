#ifndef CAMERA_H_
    #define CAMERA_H_

    #include "vec3.h"

struct camera {
    // settings, filled by the host before camera_init
    int image_width;
    int image_height;
    int samples_per_pixel;      // host only: total, split into passes
    int samples_per_pass;       // samples taken by one kernel launch
    float vfov;                 // vertical field of view, degrees
    struct vec3 lookfrom;
    struct vec3 lookat;
    struct vec3 vup;
    float defocus_angle;        // degrees, 0 = no defocus blur
    float focus_dist;           // distance from lookfrom to the sharp plane

    // computed by camera_init, read by the kernel
    struct vec3 center;
    struct vec3 pixel00_loc;
    struct vec3 pixel_delta_u;
    struct vec3 pixel_delta_v;
    struct vec3 defocus_disk_u;
    struct vec3 defocus_disk_v;
};

#ifdef __cplusplus
extern "C" {
#endif

// host only
void camera_init(struct camera *cam);

#ifdef __cplusplus
}
#endif

#endif /* !CAMERA_H_ */
