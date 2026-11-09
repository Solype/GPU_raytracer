// Device program, built with "-I kernel_gpu".
// Order matters: each file uses functions defined in the files above it.
#include "my_random.c"
#include "vec3.c"
#include "hits.c"
#include "sphere.c"
#include "materials.c"
#include "main.cl"
