//
// Created by Fernando on 14/09/2026.
//
#include <stdbool.h>
#include "gestor_de_parametros.h"

/* ---- ********************************* Constantes termodinámicas del sistema ******************************** ---- */
const double          temp_ini =      1.0;  //Se recomienda que no llegue a cero
const double          temp_fin =      3.0;  //Se recomienda mantenerlo no tan alejado del punto crítico
const int             num_temp =      200;  //No debe de ser cero
const double h_cpo_mgntc_exter =      0.0;  //Se recomienda mantener en cero para comparar con Onsager
const double       J_intns_aco =      1.0;  //Se recomienda mantener en uno para comparar con Onsager

/* ---- ***** Constantes de control para la termalización, estadística del sistema e inicio de simulación ****** ---- */
const uint64_t         semilla = 22011997;  //Propuesta de semilla
const int         num_pasos_mc =    12000;  //Número de pasos para Montecarlo por cada temperatura
const int  num_pasos_termaliza =     6000;  //En futuras versiones se modificará para que sea más eficiente
const int             ejec_ini =        1;  //Actualizar tras cada simulación, ya que puede sobreescribir
const int             ejec_fin =      100;  //Actualizar tras cada simulación, ya que puede sobreescribir
const bool        genera_mapas =     true;  //Se recomienda usar apagado para hacer pruebas
const int        num_hilos_max =        8;  //Se recomienda usar la mitad de los hilos para no saturar el procesador

double calcula_temp_star(int i_temp) {
    if (num_temp <= 1) return temp_ini;
    return temp_ini + i_temp * (temp_fin - temp_ini)/(double)(num_temp - 1);
}