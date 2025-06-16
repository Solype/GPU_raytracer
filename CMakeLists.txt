cmake_minimum_required(VERSION 3.10)

# Nom du projet et version
project(app VERSION 1.0 LANGUAGES CXX C)

# Spécifier la norme C++ (exemple C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Ajouter l'exécutable (remplace "main.cpp" par le nom de ton fichier source)
add_executable(app
    main.cpp
    GPUClasses/src/GPUProgram.cpp
    GPUClasses/src/GPUPlateform.cpp
    GPUClasses/src/GPUDevice.cpp
    GPUClasses/src/GPUContext.cpp
    GPUClasses/src/GPUQueue.cpp
    GPUClasses/src/GPUKernel.cpp
    kernel_gpu/vec3.c
    kernel_gpu/camera.c
    kernel_gpu/my_random.c
)
target_compile_definitions(app PRIVATE __cmakelist)
include_directories(${CMAKE_SOURCE_DIR}/GPUClasses/include)

find_library(OpenCL_LIBRARY OpenCL)
target_link_libraries(app PRIVATE ${OpenCL_LIBRARY})

file(COPY ${CMAKE_SOURCE_DIR}/kernel_gpu DESTINATION ${CMAKE_BINARY_DIR})
