//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_2D_METROPOLIS_GESTOR_DE_DATOS_DE_SALIDA_H
#define MODELO_ISING_2D_METROPOLIS_GESTOR_DE_DATOS_DE_SALIDA_H

#include <stdio.h>
#include "gestor_de_parametros.h"

/* ---- **************************************** Generador de carpetas ***************************************** ---- */
void crea_carpetas_salida(void);
void reporta_error_archivo(char *nombre_archivo);

/* ---- *********************************** Estadística por paso Monte Carlo *********************************** ---- */
FILE *crea_estadistica_por_paso_montecarlo(int i_ejec, int i_temp);
void escribe_estadistica_por_paso_montecarlo(FILE *archivo_estadistica, int i_mc, double ener_i,
     double magn_i, double accpt);

/* ---- ****************************** Resultados finales obtenidos por simulación ***************************** ---- */
FILE *crea_archivo_resultados_finales(int i_ejec);
void escribe_resultados_finales(FILE *archivo_resultados, double temp_star, double ener_prom,
     double susc_ter, double magn_prom, double susc_magn);

/* ---- ******************************** Reporte por cada temperatura simulada ********************************* ---- */
FILE *crea_reporte_de_simulacion(int i_ejec, int i_temp);
void escribe_reporte_de_simulacion(FILE *archivo_reporte, int i_ejec, int i_temp, double temp_star,
     double ener_prom, double susc_ter, double magn_prom, double susc_magn);

/* ---- ************************************ Configuración final de espines ************************************ ---- */
FILE *crea_configuracion_final_de_spines(int i_ejec, int i_temp);
void escribe_configuracion_final_de_spines(FILE *archivo_spines, const red_ising_t *ejecucion);

/* ---- ****************** Configuración del generador de imágenes de microestados vía GNUPLOT ***************** ---- */
void inicia_generador_imagenes_spines(void);
void genera_imagenes_spines(int i_ejec, int i_temp);
void finaliza_imagenes_spines(void);
void ejecuta_generador_imagenes_spines(void);

/* ---- ********************* Datos de salida en pantalla para notar el avance en paralelo ********************* ---- */
void cadena_porcentaje(char caracter, int num);
void imprime_avance_ejecucion(int i_ejec, int i_temp);
/* ---- ******************************************************************************************************** ---- */

#endif //MODELO_ISING_2D_METROPOLIS_GESTOR_DE_DATOS_DE_SALIDA_H
