//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_2D_METROPOLIS_GESTOR_DE_OBSERVABLES_H
#define MODELO_ISING_2D_METROPOLIS_GESTOR_DE_OBSERVABLES_H

#include "gestor_de_parametros.h"

int suma_de_vecinos (const red_ising_t *ejecucion, int i);
double calcula_magnetizacion_por_spin(const red_ising_t *ejecucion);
double calcula_energia_total (const red_ising_t *ejecucion);

#endif //MODELO_ISING_2D_METROPOLIS_GESTOR_DE_OBSERVABLES_H
