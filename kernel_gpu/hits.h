#ifndef HITS_H_
    #define HITS_H_

// device only (float3)
struct hits {
    float t;
    float3 point;
    float3 normal;
    int front_face;
    uint mat_id;
};

#endif /* !HITS_H_ */
