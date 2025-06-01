#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <fcntl.h>
#include <unistd.h>

#include "GPUClasses/GPUProgram.hpp"
#include "GPUClasses/GPUPlateform.hpp"
#include "GPUClasses/GPUDevice.hpp"
#include "GPUClasses/GPUQueue.hpp"
#include "GPUClasses/GPUKernel.hpp"

#include "kernel_gpu/camera.h"
#include "kernel_gpu/primitive.h"
#include "kernel_gpu/sphere.h"

std::vector<struct primitive> create_primitives(void)
{
    std::vector<struct primitive> A(2);
    struct sphere s1 = (struct sphere){.origin = { .x = 0.0, .y = 0.0, .z = -1}, .radius = 0.5};
    struct sphere s2 = (struct sphere){.origin = { .x = 0.0, .y = -100.5, .z = -1}, .radius = 100.0};
    struct primitive p1 = (struct primitive){.type = SHPERE, .data = s1};
    struct primitive p2 = (struct primitive){.type = SHPERE, .data = s2};

    A[0] = p1;
    A[1] = p2;

    return A;
}

void write_ppm(const std::string& filename, int width, int height, const std::vector<struct vec3>& colors)
{
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Failed to open file for writing");
    }
    out << "P3\n" << width << ' ' << height << "\n255\n";
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            auto& c = colors[width * j + i];
            out << static_cast<unsigned int>(static_cast<unsigned char>(c.x)) << ' '
                << static_cast<unsigned int>(static_cast<unsigned char>(c.y)) << ' '
                << static_cast<unsigned int>(static_cast<unsigned char>(c.z)) << ' ';
        }
        out << '\n';
    }
    out.close();
}

int main()
{
    gpu::Plateform plateform;
    gpu::Device device;
    gpu::Program program;
    gpu::Context context;
    gpu::Queue queue;
    gpu::Kernel kernel;
    cl_int err;
    struct camera cam;
    size_t width = 400, height;

    double aspect_ratio = 16.0 / 9.0;
    double focal_length = 1.0;
    height = (int)((width) / aspect_ratio);

    create_camera(&cam, width, height,  focal_length);
    auto primitives = create_primitives();
    unsigned int nb_primitives = primitives.size();
    std::vector<struct vec3> colors(width * height);

    if (plateform.load_noexcept() == false
        || device.load_noexcept(plateform) == false
        || context.init_noexcept(device) == false
        || queue.init_noexcep(context, device) == false
    ) {
        return 84;
    }
    program.add_module("./kernel_gpu/entry_point.c");
    if (program.compile_noexcept(context, device) == false
        || kernel.init_noexcept(program, "compute_ray") == false) {
        return 84;
    }

    context.add_ressource(0, primitives.data(), sizeof(struct primitive) * primitives.size(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    context.add_ressource(1, &nb_primitives, sizeof(unsigned int), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    context.add_ressource(2, &cam, sizeof(cam), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    context.add_ressource(3, nullptr, sizeof(struct vec3) * colors.size(), CL_MEM_WRITE_ONLY);
    kernel.load_params(context);

    queue.run(kernel, width * height);
    queue.read_result(context, 3, sizeof(struct vec3) * colors.size(), colors.data());
    write_ppm("image.ppm", width, height, colors);

    return 0;
}
