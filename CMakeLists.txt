cmake_minimum_required(VERSION 3.20)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de compilacion" FORCE)
endif()

project(Modelo_Ising_2D_Metropolis C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


find_package(OpenMP REQUIRED)

add_library(pcg_static STATIC third_party/pcg-c-basic-0.9/pcg_basic.c)
target_include_directories(pcg_static PUBLIC third_party/pcg-c-basic-0.9)

add_executable(Modelo_Ising_2D_Metropolis main.c
        include/gestor_de_datos_de_salida.h
        include/gestor_de_ejecuciones.h
        include/gestor_de_red.h
        include/gestor_de_observables.h
        include/gestor_de_parametros.h
        include/gestor_metropolis.h
        include/gestor_tabla_boltzmann.h
        src/gestor_de_datos_de_salida.c
        src/gestor_de_ejecuciones.c
        src/gestor_de_red.c
        src/gestor_de_observables.c
        src/gestor_de_parametros.c
        src/gestor_metropolis.c
        src/gestor_tabla_boltzmann.c
)

target_include_directories(Modelo_Ising_2D_Metropolis PRIVATE include)
target_link_libraries(Modelo_Ising_2D_Metropolis PRIVATE pcg_static OpenMP::OpenMP_C m)

if(MINGW OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(Modelo_Ising_2D_Metropolis PRIVATE -fopenmp)
    target_link_options(Modelo_Ising_2D_Metropolis PRIVATE -fopenmp)
endif()


#Configuración para uso de advertencias y librería matematica
if(MSVC)
    target_compile_options(Modelo_Ising_2D_Metropolis PRIVATE /W4)
else()
    target_compile_options(Modelo_Ising_2D_Metropolis PRIVATE -Wall -Wextra)
endif()

if(UNIX OR MINGW)
    target_link_libraries(Modelo_Ising_2D_Metropolis PRIVATE m)
endif()