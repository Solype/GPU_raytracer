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

#include "GPUProgram.hpp"
#include "GPUPlateform.hpp"
#include "GPUDevice.hpp"
#include "GPUQueue.hpp"
#include "GPUKernel.hpp"

#include "RayTracerRegistry.hpp"

#include "kernel_gpu/camera.h"
#include "kernel_gpu/struct.h"
#include "kernel_gpu/sphere.h"
#include "kernel_gpu/materials.h"
#include "kernel_gpu/materials.h"

void register_the_scene(RayTracerRegistry &reg)
{
    reg.register_primitive(Sphere(0.0, -100.5, -1, 100.0), Diffuse(0.8, 0.8, 0.0));
    reg.register_primitive(Sphere(0.0, 0.0, -1.2, 0.5), Diffuse(0.1, 0.2, 0.5));

    // left
    reg.register_primitive(Sphere(-1.0, 0.0, -1.5, 0.5), Metal(0.8, 0.8, 0.8, 0.7)); // Metal(0.8, 0.6, 0.2, 0.2)); // Dielectric(1.5f));

    //right
    // reg.register_primitive(Sphere(1.0, 0.0, -1.0, 0.5), Metal(0.8, 0.6, 0.2, 1.0));

    // reg.register_primitive(Sphere(-1.0, 0.0, -1.0, 0.5), Metal(0.8, 0.8, 0.8, 0.2));
    // reg.register_primitive(Sphere(0.0, 0.75, 0.25, 0.75), Diffuse(0.8, 0.6, 0.0));
    // reg.register_primitive(Sphere(0.0, 0.0, -4.0, 3.0), Diffuse(0.8, 0.6, 0.0));
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
    width = 650;

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
    RayTracerRegistry reg;
    cl_int err;
    struct scene_data scenedata;
    size_t width;
    size_t height;

    srand((int)time(nullptr));
    scenedata.rand_seed = rand();
    set_cam(scenedata.cam, width, height);

    register_the_scene(reg);
    scenedata.nb_primitives = reg.get_primitives().size();
    scenedata.nb_materials = reg.get_materials().size();
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
    context.add_ressource(2, reg.get_primitives().data(), sizeof(struct primitive) * scenedata.nb_primitives, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    context.add_ressource(3, reg.get_materials().data(), sizeof(struct material) * scenedata.nb_materials, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    context.add_ressource(4, reg.get_primitives().data(), sizeof(struct primitive) * scenedata.nb_primitives, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR);
    kernel.load_params(context);

    queue.run(kernel, width * height);
    queue.read_result(context, 0, sizeof(struct vec3) * colors.size(), colors.data());
    write_ppm("image.ppm", width, height, colors);

    return 0;
}
