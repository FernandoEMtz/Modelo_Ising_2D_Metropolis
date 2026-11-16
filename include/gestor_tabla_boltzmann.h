//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_2D_METROPOLIS_GESTOR_TABLA_BOLTZMANN_H
#define MODELO_ISING_2D_METROPOLIS_GESTOR_TABLA_BOLTZMANN_H

typedef struct {
    double *valores;    //Valores para la tabla boltzmann
    int tam;            //Almacenamiento dinamico del tamaño del arreglo, z+1
}tabla_boltzmann_t;

tabla_boltzmann_t calculo_tabla_boltzmann(double temp_star);
double consulta_tabla_boltzmann(const tabla_boltzmann_t *tabla, int suma_spin_vecinos, int spin_actual);
void libera_memoria_tabla_boltzmann(tabla_boltzmann_t *tabla);
void pre_calcula_tabla_boltzmann(tabla_boltzmann_t *tablas_boltzmann);
void libera_memoria_tabla_temperatura_boltzmann(tabla_boltzmann_t *tablas_boltzmann);

#endif //MODELO_ISING_2D_METROPOLIS_GESTOR_TABLA_BOLTZMANN_H
