cmake_minimum_required(VERSION 3.10)

# Nom du projet et version
project(app VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Code hôte uniquement : le kernel (kernel_gpu/*.c, main.cl) est compilé
# par OpenCL au lancement. camera.c est le seul fichier C partagé.
add_executable(app
    main.cpp
    GPUClasses/src/GPUProgram.cpp
    GPUClasses/src/GPUPlatform.cpp
    GPUClasses/src/GPUDevice.cpp
    GPUClasses/src/GPUContext.cpp
    GPUClasses/src/GPUQueue.cpp
    GPUClasses/src/GPUKernel.cpp
    RayTracerPrimitiveCpp/src/RayTracerRegistry.cpp
    RayTracerPrimitiveCpp/src/Materials.cpp
    RayTracerPrimitiveCpp/src/Primitives.cpp
    kernel_gpu/camera.c
)
target_compile_definitions(app PRIVATE CL_TARGET_OPENCL_VERSION=220)
target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/GPUClasses/include
    ${CMAKE_SOURCE_DIR}/RayTracerPrimitiveCpp/include
    ${CMAKE_SOURCE_DIR}/kernel_gpu
)

find_library(OpenCL_LIBRARY OpenCL)
target_link_libraries(app PRIVATE ${OpenCL_LIBRARY})

# Recopié à chaque build : une modif du kernel est prise sans relancer cmake
add_custom_target(kernels ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/kernel_gpu ${CMAKE_BINARY_DIR}/kernel_gpu
)
