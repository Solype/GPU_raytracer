#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
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

// Ray Tracing in One Weekend, chapters 11.5 to 13: diffuse, fuzzy metal,
// hollow glass (air bubble inside a glass sphere), positioned camera, defocus blur
void scene_materials(RayTracerRegistry &reg, struct camera &cam)
{
    reg.register_primitive(Sphere(0.0, -100.5, -1.0, 100.0), Diffuse(0.8, 0.8, 0.0));
    reg.register_primitive(Sphere(0.0, 0.0, -1.2, 0.5), Diffuse(0.1, 0.2, 0.5));
    reg.register_primitive(Sphere(-1.0, 0.0, -1.0, 0.5), Dielectric(1.5f));
    // bubble: air (1.0) inside glass (1.5) seen from the glass side
    reg.register_primitive(Sphere(-1.0, 0.0, -1.0, 0.4), Dielectric(1.0f / 1.5f));
    reg.register_primitive(Sphere(1.0, 0.0, -1.0, 0.5), Metal(0.8, 0.6, 0.2, 1.0));

    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.vfov = 20;
    cam.lookfrom = {-2, 2, 1};
    cam.lookat = {0, 0, -1};
    cam.vup = {0, 1, 0};
    cam.defocus_angle = 10.0;
    cam.focus_dist = 3.4;
}

// Ray Tracing in One Weekend, chapter 14: the cover image
void scene_final(RayTracerRegistry &reg, struct camera &cam)
{
    // fixed seed: the same spheres at every run
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> rnd(0.0f, 1.0f);

    reg.register_primitive(Sphere(0, -1000, 0, 1000), Diffuse(0.5, 0.5, 0.5));
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float choose_mat = rnd(gen);
            float x = a + 0.9f * rnd(gen);
            float z = b + 0.9f * rnd(gen);

            // keep the space around the big metal sphere free
            if (std::hypot(x - 4.0f, z) <= 0.9f) {
                continue;
            }
            if (choose_mat < 0.8f) {
                float r = rnd(gen) * rnd(gen);
                float g = rnd(gen) * rnd(gen);
                float bl = rnd(gen) * rnd(gen);
                reg.register_primitive(Sphere(x, 0.2, z, 0.2), Diffuse(r, g, bl));
            } else if (choose_mat < 0.95f) {
                float r = 0.5f + 0.5f * rnd(gen);
                float g = 0.5f + 0.5f * rnd(gen);
                float bl = 0.5f + 0.5f * rnd(gen);
                reg.register_primitive(Sphere(x, 0.2, z, 0.2), Metal(r, g, bl, 0.5f * rnd(gen)));
            } else {
                reg.register_primitive(Sphere(x, 0.2, z, 0.2), Dielectric(1.5f));
            }
        }
    }
    reg.register_primitive(Sphere(0, 1, 0, 1.0), Dielectric(1.5f));
    reg.register_primitive(Sphere(-4, 1, 0, 1.0), Diffuse(0.4, 0.2, 0.1));
    reg.register_primitive(Sphere(4, 1, 0, 1.0), Metal(0.7, 0.6, 0.5, 0.0));

    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.vfov = 20;
    cam.lookfrom = {13, 2, 3};
    cam.lookat = {0, 0, 0};
    cam.vup = {0, 1, 0};
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;
}

// linear -> gamma 2 -> [0, 255]; NaN and negatives give 0
unsigned int to_byte(float linear)
{
    if (!(linear > 0.0f)) {
        return 0;
    }
    return (unsigned int)(std::sqrt(std::min(linear, 1.0f)) * 255.0f + 0.5f);
}

// accum holds the sum of `samples` linear samples per pixel
void write_ppm(const std::string& filename, int width, int height, const std::vector<struct vec3>& accum, int samples)
{
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Failed to open file for writing");
    }
    out << "P3\n" << width << ' ' << height << "\n255\n";
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            auto& c = accum[width * j + i];
            out << to_byte(c.x / samples) << ' '
                << to_byte(c.y / samples) << ' '
                << to_byte(c.z / samples) << ' ';
        }
        out << '\n';
    }
}

void usage(char const *prog)
{
    std::cout << "Usage: " << prog << " [-h|--help] [scene]\n"
        << "Renders a scene on the first OpenCL device and writes image.ppm.\n"
        << "Run it from the build directory: kernels are loaded from ./kernel_gpu.\n\n"
        << "Arguments:\n"
        << "  scene       final (default): book 1 cover, ~480 random spheres, 1200px, 500 spp\n"
        << "              materials: diffuse, metal, hollow glass, defocus blur, 400px, 100 spp\n\n"
        << "Options:\n"
        << "  -h, --help  show this help and exit\n\n"
        << "Environment variables: none\n\n"
        << "Example:\n"
        << "  " << prog << " materials\n";
}

int main(int argc, char **argv)
{
    std::map<std::string, void (*)(RayTracerRegistry &, struct camera &)> const scenes = {
        {"final", scene_final},
        {"materials", scene_materials},
    };
    std::string scene_name = argc > 1 ? argv[1] : "final";

    if (scene_name == "-h" || scene_name == "--help") {
        usage(argv[0]);
        return 0;
    }
    if (argc > 2 || scenes.count(scene_name) == 0) {
        usage(argv[0]);
        return 84;
    }

    gpu::Platform platform;
    gpu::Device device;
    gpu::Program program;
    gpu::Context context;
    gpu::Queue queue;
    gpu::Kernel kernel;
    RayTracerRegistry reg;
    struct scene_data scenedata = {};
    struct camera &cam = scenedata.cam;

    srand((unsigned int)time(nullptr));
    scenedata.rand_seed = rand();
    scenes.at(scene_name)(reg, cam);
    cam.image_height = std::max(1, (int)(cam.image_width / (16.0 / 9.0)));
    // ~10 samples per launch keeps each launch well under the driver watchdog
    cam.samples_per_pass = std::min(10, cam.samples_per_pixel);
    camera_init(&cam);
    int passes = (cam.samples_per_pixel + cam.samples_per_pass - 1) / cam.samples_per_pass;

    auto &primitives = reg.get_primitives();
    auto &materials = reg.get_materials();
    scenedata.nb_primitives = primitives.size();
    scenedata.nb_materials = materials.size();
    std::vector<struct vec3> accum((size_t)cam.image_width * cam.image_height, {0.0f, 0.0f, 0.0f});

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

    // ids are the compute_ray argument indexes; accum starts zeroed (copied from host)
    if (context.add_resource(0, accum.data(), sizeof(struct vec3) * accum.size(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR) == false
        || context.add_resource(1, &scenedata, sizeof(scenedata), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR) == false
        || context.add_resource(2, primitives.data(), sizeof(struct primitive) * primitives.size(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR) == false
        || context.add_resource(3, materials.data(), sizeof(struct material) * materials.size(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR) == false
        || kernel.load_params(context) == false
    ) {
        return 84;
    }
    std::cerr << scene_name << ": " << cam.image_width << "x" << cam.image_height << ", "
        << primitives.size() << " primitives, " << passes * cam.samples_per_pass << " spp" << std::endl;
    // render time: GPU passes + readback; excludes kernel build and image writing
    auto render_start = std::chrono::steady_clock::now();
    for (cl_uint pass = 0; pass < (cl_uint)passes; ++pass) {
        if (kernel.set_arg(4, sizeof(pass), &pass) == false
            || queue.run(kernel, accum.size()) == false) {
            return 84;
        }
        std::cerr << "\rpass " << pass + 1 << "/" << passes << std::flush;
    }
    std::cerr << std::endl;
    if (queue.read_result(context, 0, sizeof(struct vec3) * accum.size(), accum.data()) == false) {
        return 84;
    }
    std::chrono::duration<double> render_time = std::chrono::steady_clock::now() - render_start;
    std::cerr << "render time: " << render_time.count() << " s" << std::endl;
    write_ppm("image.ppm", cam.image_width, cam.image_height, accum, passes * cam.samples_per_pass);
    return 0;
}
