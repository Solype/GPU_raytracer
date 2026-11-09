#include <cstdlib>
#include <ctime>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "GPUContext.hpp"
#include "GPUDevice.hpp"
#include "GPUKernel.hpp"
#include "GPUPlatform.hpp"
#include "GPUProgram.hpp"
#include "GPUQueue.hpp"

#include "RayTracerRegistry.hpp"
#include "struct.h"

void register_the_scene(RayTracerRegistry &reg)
{
    reg.register_primitive(Sphere(0.0, -100.5, -1, 100.0), Diffuse(0.8, 0.8, 0.0));
    reg.register_primitive(Sphere(0.0, 0.0, -1.2, 0.5), Diffuse(0.1, 0.2, 0.5));

    // left
    reg.register_primitive(Sphere(-1.0, 0.0, -1.0, 0.5), Metal(0.8, 0.8, 0.8, 0.3)); // Metal(0.8, 0.6, 0.2, 0.2)); // Dielectric(1.5f));

    //right
    reg.register_primitive(Sphere(1.0, 0.0, -1.0, 0.5), Dielectric(1.5f)); //Metal(0.8, 0.6, 0.2, 1.0));

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
    height = (size_t)(width / aspect_ratio);
    create_camera(&cam, width, height, focal_length);
    cam.sample_pixel = 100;
}

int main()
{
    gpu::Platform platform;
    gpu::Device device;
    gpu::Program program;
    gpu::Context context;
    gpu::Queue queue;
    gpu::Kernel kernel;
    RayTracerRegistry reg;
    struct scene_data scenedata = {};
    size_t width;
    size_t height;

    srand((unsigned int)time(nullptr));
    scenedata.rand_seed = rand();
    set_cam(scenedata.cam, width, height);

    register_the_scene(reg);
    auto &primitives = reg.get_primitives();
    auto &materials = reg.get_materials();
    scenedata.nb_primitives = primitives.size();
    scenedata.nb_materials = materials.size();
    std::vector<struct vec3> colors(width * height);

    if (platform.load_noexcept() == false
        || device.load_noexcept(platform) == false
        || context.init_noexcept(device) == false
        || queue.init_noexcept(context, device) == false
    ) {
        return 84;
    }
    // paths are relative to the working directory: run from the build dir
    program.add_module("./kernel_gpu/entry_point.c");
    if (program.compile_noexcept(context, device, "-I kernel_gpu") == false
        || kernel.init_noexcept(program, "compute_ray") == false) {
        return 84;
    }

    // ids are the compute_ray argument indexes
    if (context.add_resource(0, nullptr, sizeof(struct vec3) * colors.size(), CL_MEM_WRITE_ONLY) == false
        || context.add_resource(1, &scenedata, sizeof(scenedata), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR) == false
        || context.add_resource(2, primitives.data(), sizeof(struct primitive) * primitives.size(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR) == false
        || context.add_resource(3, materials.data(), sizeof(struct material) * materials.size(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR) == false
        || kernel.load_params(context) == false
        || queue.run(kernel, width * height) == false
        || queue.read_result(context, 0, sizeof(struct vec3) * colors.size(), colors.data()) == false
    ) {
        return 84;
    }
    write_ppm("image.ppm", width, height, colors);
    return 0;
}
