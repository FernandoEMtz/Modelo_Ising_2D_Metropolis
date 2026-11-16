//
// Created by Fernando on 14/09/2026.
//

#include "gestor_de_observables.h"

int suma_de_vecinos(const red_ising_t *ejecucion, int i) {
    int j = 0, k = 0, up = 0, dw = 0, lf = 0, rg = 0;

    j = (i-1)/N_X+1; //Obtencion de la fila
    k = (i-1)%N_X+1; //Obtencion de la columna

    up = ( j == 1 ) ? ejecucion->spin[i + N_X*(N_Y-1)] : ejecucion->spin[i - N_X];
    dw = (j == N_Y) ? ejecucion->spin[i - N_X*(N_Y-1)] : ejecucion->spin[i + N_X];
    lf = ( k == 1 ) ? ejecucion->spin[i + ( N_X - 1) ] : ejecucion->spin[i - 1];
    rg = (k == N_X) ? ejecucion->spin[i - ( N_X - 1) ] : ejecucion->spin[i + 1];

    return (up + dw + lf + rg);
}

/*Se obtiene la magnetizacion por cada spin, es un calco extensivo, para hacer la estadistica es importante dividir
* entre el número total de spines debido a que estamos hablando de la magnetizacion y la susceptibilidad
* magnetica por spin
*/
double calcula_magnetizacion_por_spin(const red_ising_t *ejecucion) {
    int i = 0;
    double mom_magn = 0;
    for(i = 1; i <= N_SP; i++) {
        mom_magn += ejecucion->spin[i];
    }
    return mom_magn/(double)N_SP;
}

/* Caculamos la energía física total del sistema mediante el hamiltoniano recordemos que se describe mediante
 *
 * H = -j * Sum_{<i, j>} s_i s_j - h_campo * Sum_i s_i
 */
double calcula_energia_total(const red_ising_t *ejecucion) {
    int i = 0;
    double inter_vec = 0.0;

    //Suma de la interacción del spin con las cargas de sus vecinos
    for(i = 1; i <= N_SP; i++) inter_vec += ejecucion->spin[i] * suma_de_vecinos(ejecucion, i);

    //Se usa 0.5 para no contar dos veces cada enlace
    double hamiltoniano = -J_intns_aco * 0.5 * inter_vec - h_cpo_mgntc_exter * (double)N_SP * calcula_magnetizacion_por_spin(ejecucion);

    return hamiltoniano;
}