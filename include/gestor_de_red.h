//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_2D_METROPOLIS_GESTOR_DE_RED_H
#define MODELO_ISING_2D_METROPOLIS_GESTOR_DE_RED_H

#include "pcg_basic.h"
#include "gestor_de_parametros.h"

void inicia_generador_pcg32(pcg32_random_t *random_local, int i_ejec);
double randomizador_uniforme(pcg32_random_t *random_local);
void inicia_spines_aleatorios(red_ising_t *ejecucion);

#endif //MODELO_ISING_2D_METROPOLIS_GESTOR_DE_RED_H
