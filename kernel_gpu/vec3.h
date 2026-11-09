#ifndef VEC3_H_
    #define VEC3_H_

// shared with the host: plain floats so the layout matches on both sides
// (an OpenCL float3 is 16 bytes, the C++ side would not match)
struct vec3 {
    float x;
    float y;
    float z;
};

#endif /* !VEC3_H_ */
