//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_2D_METROPOLIS_GESTOR_DE_PARAMETROS_H
#define MODELO_ISING_2D_METROPOLIS_GESTOR_DE_PARAMETROS_H

#include <stdbool.h>
#include "pcg_basic.h"

/* ---- *************************************** Dimensiones del sistema **************************************** ---- */
#define N_X     40                      //Número de columnas en la red
#define N_Y     40                      //Número de filas    en la red
#define N_SP    (N_X*N_Y)               //Número total de espines en la red bidimensional
#define N_VEC   4                       //Número de vecinos, en una red bidimensional son 4 vecinos
                                        //Se maneja la variable para redes más complejas para las tablas de Boltzmann

/* ---- ********************************* Constantes termodinámicas del sistema ******************************** ---- */
extern const double          temp_ini;  //Temperatura reducida inicial del sistema
extern const double          temp_fin;  //Temperatura reducida final del sistema
extern const int             num_temp;  //Número total de temperaturas para realizar el barrido
extern const double h_cpo_mgntc_exter;  //Campo magnético externo reducido
extern const double       J_intns_aco;  // Constante de intensidad de acoplamiento

/* ---- ***** Constantes de control para la termalización, estadística del sistema e inicio de simulación ****** ---- */

extern const int                 i_mc;  //Número de pasos para Montecarlo por cada temperatura
extern const int              n_start;  //Número de pasos en el que comienza a contar la estadística
extern const int              ini_sim;  //Límite inferior a las simulaciones de la ejecución en curso
extern const int              fin_sim;  //Límite superior a las simulaciones de la ejecución en curso
extern const bool           plot_mapa;  //Bandera de control para generar las imagenes de los mapas de espines
extern const int                hilos;  //Número de hilos a utilizar para OpenMP

/* ---- *************** Estructura de almacenamiento para cada ejecución del arreglo de espines **************** ---- */
typedef struct {
    int spin[N_SP+1];       //Nunca se usa spin[0] CORREGIR
    pcg32_random_t gen_rng; //Almacenamiento del generador de número aleatorio por ejecución
}red_ising_t;
/* ---- ******************************************************************************************************** ---- */

#endif //MODELO_ISING_2D_METROPOLIS_GESTOR_DE_PARAMETROS_H
