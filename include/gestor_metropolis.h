//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_2D_METROPOLIS_GESTOR_METROPOLIS_H
#define MODELO_ISING_2D_METROPOLIS_GESTOR_METROPOLIS_H

#include <stdio.h>
#include "gestor_de_parametros.h"
#include "gestor_tabla_boltzmann.h"

//Este struct almacenará los resultados termodinamicos promedio que guardaremos en las diversas salidas
typedef struct {
    double ener_prom;   //energía promedio
    double magn_prom;   //magnetizacion promedio
    double susc_ter;    //susceptibilidad termica -> c_v
    double susc_magn;   //susceptibilidad magnetica -> chi
}resultados_montecarlo_t;

resultados_montecarlo_t ejecuta_montecarlo(red_ising_t *ejecucion, const tabla_boltzmann_t *tabla, FILE *archivo_estadistica, double ener_ini, double temp_star);


#endif //MODELO_ISING_2D_METROPOLIS_GESTOR_METROPOLIS_H
