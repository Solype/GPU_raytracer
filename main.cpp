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
#include "kernel_gpu/struct.h"
#include "kernel_gpu/sphere.h"
#include "kernel_gpu/material_diffuse.h"
#include "kernel_gpu/material_metal.h"

std::vector<struct material> create_materials(void)
{
    struct material_diffuse ground = (struct material_diffuse) {.albedo = {.x = 0.8f, .y = 0.8f, .z = 0.0f}};
    struct material_diffuse center = (struct material_diffuse) {.albedo = {.x = 0.1f, .y = 0.2f, .z = 0.5f}};
    struct material_metal left = (struct material_metal) { .albedo = {.x = 0.8, .y = 0.8, .z = 0.8}};
    struct material_metal right = (struct material_metal) { .albedo = {.x = 0.8, .y = 0.6, .z = 0.2}};

    std::vector<struct material> m(4);
    m[0].type = DIFFUSE;
    m[1].type = DIFFUSE;
    m[2].type = METAL;
    m[3].type = METAL;

    m[0].data.diffuse = ground;
    m[1].data.diffuse = center;
    m[2].data.metal = left;
    m[3].data.metal = right;
    return m;
}

std::vector<struct primitive> create_primitives(void)
{
    std::vector<struct primitive> A(4);
    struct sphere ground = (struct sphere){.origin = { .x = 0.0, .y = -100.5, .z = -1}, .radius = 100.0};
    struct sphere center = (struct sphere){.origin = { .x = 0.0, .y = 0.0, .z = -1.2}, .radius = 0.5};
    struct sphere left = (struct sphere){.origin = { .x = -1.0, .y = 0.0, .z = -1}, .radius = 0.5};
    struct sphere right = (struct sphere){.origin = { .x = 1.0, .y = 0.0, .z = -1}, .radius = 0.5};

    struct primitive p_ground = (struct primitive){.type = SHPERE, .mat_id = 0, .data = ground,};
    struct primitive p_center = (struct primitive){.type = SHPERE, .mat_id = 1, .data = center,};
    struct primitive p_left = (struct primitive){.type = SHPERE, .mat_id = 2, .data = left,};
    struct primitive p_right = (struct primitive){.type = SHPERE, .mat_id = 3, .data = right,};

    A[0] = p_ground;
    A[1] = p_center;
    A[2] = p_left;
    A[3] = p_right;

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

void set_cam(struct camera &cam, size_t &width, size_t &height)
{
    width = 800;

    double aspect_ratio = 16.0 / 9.0;
    double focal_length = 1.0;
    height = (int)((width) / aspect_ratio);
    create_camera(&cam, width, height,  focal_length);
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
    struct scene_data scenedata;
    size_t width;
    size_t height;

    srand((int)time(nullptr));
    scenedata.rand_seed = rand();
    set_cam(scenedata.cam, width, height);

    auto primitives = create_primitives();
    auto materials = create_materials();
    scenedata.nb_primitives = primitives.size();
    scenedata.nb_materials = materials.size();
    std::cout << width << " " << height << std::endl;
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

    context.add_ressource(0, nullptr, sizeof(struct vec3) * colors.size(), CL_MEM_WRITE_ONLY);
    context.add_ressource(1, &scenedata, sizeof(scenedata), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    context.add_ressource(2, primitives.data(), sizeof(struct primitive) * primitives.size(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    context.add_ressource(3, materials.data(), sizeof(struct material) * materials.size(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    context.add_ressource(4, primitives.data(), sizeof(struct primitive) * primitives.size(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    kernel.load_params(context);

    queue.run(kernel, width * height);
    queue.read_result(context, 0, sizeof(struct vec3) * colors.size(), colors.data());
    write_ppm("image.ppm", width, height, colors);

    return 0;
}
