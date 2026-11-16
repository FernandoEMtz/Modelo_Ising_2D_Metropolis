cmake_minimum_required(VERSION 3.20)
project(Modelo_Ising_2D_Metropolis C)

set(CMAKE_C_STANDARD 11)

file(GLOB pcg_source "third_party/pcg-c-basic-0.9/pcg_basic.c")
add_library(pcg_static STATIC ${pcg_source})
target_include_directories(pcg_static PUBLIC third_party/pcg-c-basic-0.9)

add_executable(Modelo_Ising_2D_Metropolis main.c
        src/gestor_de_parametros.c
        include/gestor_de_parametros.h)

target_include_directories(Modelo_Ising_2D_Metropolis PRIVATE include)
target_link_libraries(Modelo_Ising_2D_Metropolis PRIVATE pcg_static)