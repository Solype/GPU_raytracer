#include "GPUProgram.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

gpu::Program::Program() :
    program(nullptr)
{}

gpu::Program::~Program()
{
    if (this->program != nullptr) {
        clReleaseProgram(this->program);
    }
}

int gpu::Program::add_module(std::string const &path)
{
    this->_modules_to_load.push_back(path);
    return 0;
}

bool gpu::Program::compile_noexcept(gpu::Context const &context, gpu::Device const &device) noexcept
{
    std::string entire_program;
    cl_int err;

    for (auto const &elem : this->_modules_to_load) {
        std::ifstream stream(elem);
        if (!stream) {
            std::cerr << "Failed to open file: " << elem << std::endl;
            continue;
        }
        std::stringstream buffer;
        buffer << stream.rdbuf();
        entire_program += buffer.str();
    }
    const char *source = entire_program.c_str();
    if (this->program != nullptr) {
        clReleaseProgram(this->program);
    }
    this->program = clCreateProgramWithSource(context.get_context(), 1, &source, nullptr, &err);
    if (err != CL_SUCCESS) {
        std::cerr << "clCreateProgramWithSource failed: " << err << std::endl;
        return false;
    }
    err = clBuildProgram(program, 1, &device.get_device(), nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device.get_device(), CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
        std::vector<char> log(log_size);
        clGetProgramBuildInfo(program, device.get_device(), CL_PROGRAM_BUILD_LOG, log_size, log.data(), nullptr);
        std::cerr << "Erreur compilation kernel:\n" << log.data() << std::endl;
        return false;
    }
    return true;
}

void gpu::Program::compile(gpu::Context const &context, gpu::Device const &device)
{
    if (this->compile_noexcept(context, device) == false) {
        throw std::runtime_error("[GPU] failed to compile");
    }
}

cl_program const &gpu::Program::get_program() const
{
    return this->program;
}
