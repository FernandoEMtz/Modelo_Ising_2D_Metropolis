//
// Created by Fernando on 14/09/2026.
//

#include <math.h>
#include "gestor_de_red.h"
#include "gestor_de_datos_de_salida.h"
#include "gestor_de_observables.h"
#include "gestor_metropolis.h"

resultados_montecarlo_t ejecuta_montecarlo(red_ising_t *ejecucion, const tabla_boltzmann_t *tabla, FILE *archivo_estadistica, double ener_ini, double temp_star) {
    int i_mc = 0, i_nsp = 0, rand_spin = 0, suma_spin_vecinos = 0;
    int i_acc = 0, i_rec = 0;
    double factor_boltzmann = 0.0, ener_sis = ener_ini, delta_e = 0.0, accpt = 0.0, ener_i = 0.0, magn_i = 0.0;
    double ener_prom = 0.0, ener_prom2 = 0.0, magn_prom = 0.0, magn_prom2 = 0.0, var_ener = 0.0, var_magn = 0.0;
    double susc_ter = 0.0, susc_magn = 0.0;

    resultados_montecarlo_t resultado_mc;

    //Montecarlo
    for (i_mc = 1; i_mc <= num_pasos_mc; i_mc++) {
        //reseteo de los acumuladores de aceptacion y rechazo
        i_acc = i_rec = 0;

        //ciclo de spines
        for (i_nsp = 1; i_nsp <= N_SP; i_nsp++) {

            //Obtencion de un spin aleatorio entre 1 y N_SP
            rand_spin = (int)(randomizador_uniforme(&ejecucion->gen_rng) * N_SP) +1;
            suma_spin_vecinos = suma_de_vecinos(ejecucion, rand_spin);
            //metropolis, recordemos que el factor solo consulta a exp(-DeltaE/T);
            factor_boltzmann = consulta_tabla_boltzmann(tabla, suma_spin_vecinos, ejecucion->spin[rand_spin]);
            //evaluacion de metropolis, usando el generador local de la ejecucion
            if (randomizador_uniforme(&ejecucion->gen_rng) < factor_boltzmann) {
                //aceptacion: se voltea spin
                //Delta E = 2 * s_i * (J * suma_vecinos + h_campo)
                delta_e = 2.0 * (double)ejecucion->spin[rand_spin] * ( J_intns_aco * (double)suma_spin_vecinos + h_cpo_mgntc_exter);
                //cambiamos la orientacion del spin, no necesitamos calcular nuevamente toda la enrgía, solo consideramos
                //su aportacion
                ejecucion->spin[rand_spin] = -ejecucion->spin[rand_spin];
                ener_sis += delta_e;
                i_acc ++;
            } else {
                i_rec ++;
            }
        }

        accpt = (double)i_acc / (double)(i_acc + i_rec);
        ener_i = ener_sis/(double)N_SP;
        magn_i = calcula_magnetizacion_por_spin(ejecucion);

        escribe_estadistica_por_paso_montecarlo(archivo_estadistica, i_mc, ener_i, magn_i, accpt);
        //Ya no mas imprime estadistica

        if (i_mc > num_pasos_termaliza) {
            ener_prom += ener_i;
            magn_prom += fabs(magn_i);
            ener_prom2 += ener_i * ener_i;
            magn_prom2 += magn_i * magn_i;
        }
    }

    //Promedios finales y magnitudes termodinamicas
    ener_prom  /= (double)(num_pasos_mc - num_pasos_termaliza);
    magn_prom  /= (double)(num_pasos_mc - num_pasos_termaliza);
    ener_prom2 /= (double)(num_pasos_mc - num_pasos_termaliza);
    magn_prom2 /= (double)(num_pasos_mc - num_pasos_termaliza);

    var_ener = ener_prom2 - ener_prom * ener_prom;
    var_magn = magn_prom2 - magn_prom * magn_prom;

    //TODO revisar el teorema de fluctuacion-disipacion, es ahi donde se explica lo que estamos haciendo aqui
    susc_ter  = N_SP * var_ener/(temp_star * temp_star);
    susc_magn = N_SP * var_magn/temp_star;

    resultado_mc.ener_prom = ener_prom;
    resultado_mc.magn_prom = magn_prom;
    resultado_mc.susc_ter  = susc_ter;
    resultado_mc.susc_magn = susc_magn;

    return resultado_mc;
}