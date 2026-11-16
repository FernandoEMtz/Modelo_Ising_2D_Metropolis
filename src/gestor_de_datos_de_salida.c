//
// Created by Fernando on 14/09/2026.
//
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "gestor_de_datos_de_salida.h"
#include "gestor_de_parametros.h"

static FILE *gnuplot_spines = NULL;

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(ruta) _mkdir(ruta)
    #define POPEN(mcd, mode) _popen(mcd, mode)
    #define PCLOSE(pipe) _pclose(pipe)
    #define SEPARACION "\\"
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define MKDIR(ruta) mkdir(ruta, 0755)
    #define POPEN(mcd, mode) popen(mcd, mode)
    #define PCLOSE(pipe) pclose(pipe)
    #define SEPARACION "/"
#endif

#define DIR_REPORTES        "Reportes_de_simulaciones"
#define DIR_MAP_SPIN        "Configuracion_final_de_spines"
#define DIR_ESTADISTICA     "Estadistica_por_paso_Montecarlo"
#define DIR_RESULTADOS      "Resultados_finales"

static void crear_directorio_seguro(const char *ruta);

static void crear_directorio_seguro(const char *ruta) {
    int aux = MKDIR(ruta);
    if (aux == 0) {
        printf("Directorio '%s' creado exitosamente\n", ruta);
    } else {
        if (errno == EEXIST) {
            printf("El directorio '%s' ya se encuentra generado\n", ruta);
        } else {
            fprintf(stderr, "Error creando el directorio '%s' : '%s'\n", ruta,strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

void crea_carpetas_salida(void) {
    char nombre_carpeta[FILENAME_MAX];
    int i_temp = 0, i_ejec = 0;

    crear_directorio_seguro(DIR_REPORTES);
    crear_directorio_seguro(DIR_MAP_SPIN);
    crear_directorio_seguro(DIR_ESTADISTICA);
    crear_directorio_seguro(DIR_RESULTADOS);

    for (i_temp = 0; i_temp < num_temp; i_temp++) {
        snprintf(nombre_carpeta, sizeof(nombre_carpeta), "%s%sMapa_de_dominios_magneticos_%d",
            DIR_MAP_SPIN, SEPARACION, i_temp+1);
        crear_directorio_seguro(nombre_carpeta);
    }
    for (i_ejec = ejec_ini; i_ejec <= ejec_fin; i_ejec++) {
        snprintf(nombre_carpeta, sizeof(nombre_carpeta), "%s%sEstadistica_de_la_ejecucion_%d",
            DIR_ESTADISTICA, SEPARACION, i_ejec);
        crear_directorio_seguro(nombre_carpeta);
        snprintf(nombre_carpeta, sizeof(nombre_carpeta), "%s%sReporte_de_la_ejecucion_%d",
            DIR_REPORTES, SEPARACION, i_ejec);
        crear_directorio_seguro(nombre_carpeta);
    }
}


FILE *crea_reporte_de_simulacion(int i_ejec, int i_temp) {
    char nombre_archivo[FILENAME_MAX];
    FILE *archivo_reporte = NULL;

    sprintf(nombre_archivo, "Reportes_de_simulaciones/Reporte_de_la_ejecucion_%d/reporte_tecnico_%d_de_%d.dat", i_ejec, i_temp+1, num_temp);
    archivo_reporte = fopen(nombre_archivo, "w");

    if (archivo_reporte == NULL) {
        reporta_error_archivo(nombre_archivo);
    }
    return archivo_reporte;
}

FILE *crea_configuracion_final_de_spines(int i_ejec, int i_temp) {
    char nombre_archivo[FILENAME_MAX];
    FILE *archivo_spines = NULL;
    sprintf(nombre_archivo,"Configuracion_final_de_spines/Mapa_de_dominios_magneticos_%d/microestado_%d_ejecucion_%d.dat", i_temp+1, i_temp+1, i_ejec);
    archivo_spines = fopen(nombre_archivo, "w");

    if (archivo_spines == NULL) {
        reporta_error_archivo(nombre_archivo);
    }
    return archivo_spines;
}

FILE *crea_estadistica_por_paso_montecarlo(int i_ejec, int i_temp) {
    char nombre_archivo[FILENAME_MAX];
    FILE *archivo_estadistica = NULL;

    sprintf(nombre_archivo, "Estadistica_por_paso_Montecarlo/Estadistica_de_la_ejecucion_%d/montecarlo_i_temp_%d_de_%d.dat", i_ejec, i_temp+1, num_temp);
    archivo_estadistica = fopen(nombre_archivo, "w");

    if (archivo_estadistica == NULL) {
        reporta_error_archivo(nombre_archivo);
    }
    return archivo_estadistica;
}

FILE *crea_archivo_resultados_finales(int i_ejec) {
    char nombre_archivo[FILENAME_MAX];
    FILE *archivo_resultados = NULL;

    sprintf(nombre_archivo, "Resultados_finales/resultado_ejecucion_%d.dat", i_ejec);
    archivo_resultados = fopen(nombre_archivo, "w");

    if (archivo_resultados == NULL) {
        reporta_error_archivo(nombre_archivo);
    }

    return archivo_resultados;
}

void escribe_reporte_de_simulacion(FILE *archivo_reporte, int i_ejec, int i_temp, double temp_star, double ener_prom, double susc_ter, double magn_prom, double susc_magn) {
    fprintf(archivo_reporte," --- ******************************************************* --- \n");
    fprintf(archivo_reporte,"|\t\t\t\t\t\t\t\t|\n");
    fprintf(archivo_reporte,"|\t\tReporte de Simulacion de Ising\t\t\t|\n");
    fprintf(archivo_reporte,"*\t\t\t\t\t\t\t\t*\n");
    fprintf(archivo_reporte, "*\tSemilla utilizada:\t\t%" PRIu64 "\t\t*\n", semilla);
    fprintf(archivo_reporte,"*\t\t\t\t\t\t\t\t*\n");
    fprintf(archivo_reporte,"*\tEjecucion de la simulacion numero:\t%d\t\t*\n",i_ejec);
    fprintf(archivo_reporte,"*\tBarrido de temperatura :\t\t%d de %d\t*\n", i_temp+1, num_temp);
    fprintf(archivo_reporte,"*\t\t\t\t\t\t\t\t*\n");
    fprintf(archivo_reporte,"*\tTotal de pasos Montecarlo:\t\t%d\t\t*\n", num_pasos_mc);
    fprintf(archivo_reporte,"*\tPasos usados para la estadistica:\t%d\t\t*\n", num_pasos_mc - num_pasos_termaliza);
    fprintf(archivo_reporte,"*\t\t\t\t\t\t\t\t*\n");
    fprintf(archivo_reporte,"*\tTamano de la red:\t\t\t%d x %d\t\t*\n", N_X, N_Y);
    fprintf(archivo_reporte,"*\tNumero de spines:\t\t\t%d\t\t*\n", N_SP);
    fprintf(archivo_reporte,"*\t\t\t\t\t\t\t\t*\n");
    fprintf(archivo_reporte,"*\tTemperatura reducida:\t\t\t%.6f\t*\n", temp_star);
    fprintf(archivo_reporte,"*\tEnergia promedio del sistema:\t\t%.8f\t*\n", ener_prom);
    fprintf(archivo_reporte,"*\tSusceptibilidad termica (Cv):\t\t%.8f\t*\n", susc_ter);
    fprintf(archivo_reporte,"*\tMagnetizacion promedio del sistema:\t%.8f\t*\n",magn_prom);
    fprintf(archivo_reporte,"|\tSusceptibilidad magnetica (Chi):\t%.8f\t|\n",susc_magn);
    fprintf(archivo_reporte,"|\t\t\t\t\t\t\t\t|\n");
    fprintf(archivo_reporte," --- ******************************************************* --- \n");
}

void escribe_configuracion_final_de_spines(FILE *archivo_spines, const red_ising_t *ejecucion) {
    int i = 0;
    for (i = 1; i <= N_SP; i++) {
        fprintf(archivo_spines, "%d ", ejecucion->spin[i] );
        if (i % N_X == 0) fprintf(archivo_spines, "\n");
    }
}

void escribe_estadistica_por_paso_montecarlo(FILE *archivo_estadistica, int i_mc, double ener_i, double magn_i, double accpt) {
    fprintf(archivo_estadistica,"%d %15.8f %15.8f %15.8f\n", i_mc, ener_i, magn_i, accpt);
}

void escribe_resultados_finales(FILE *archivo_resultados, double temp_star, double ener_prom, double susc_ter, double magn_prom, double susc_magn) {
    fprintf(archivo_resultados, "%.8f %.8f %.8f %.8f %.8f\n", temp_star, ener_prom, susc_ter, magn_prom, susc_magn);
}

void inicia_generador_imagenes_spines(void) {
    gnuplot_spines = POPEN("gnuplot -persist", "w");

    if (gnuplot_spines == NULL) {
        fprintf(stderr, "ERROR en la generacion de imagenes por parte de gnuplot\n");
        exit(EXIT_FAILURE);
    }

    fprintf(gnuplot_spines,"set terminal pngcairo size 512, 512\n");
    fprintf(gnuplot_spines,"set size square\n");
    fprintf(gnuplot_spines,"set autoscale fix\n");
    fprintf(gnuplot_spines,"set tmargin 0\n");
    fprintf(gnuplot_spines,"set bmargin 0\n");
    fprintf(gnuplot_spines,"set lmargin 0\n");
    fprintf(gnuplot_spines,"set rmargin 0\n");
    fprintf(gnuplot_spines,"unset title\n");
    fprintf(gnuplot_spines,"unset colorbox\n");
    fprintf(gnuplot_spines,"unset key\n");
    fprintf(gnuplot_spines,"set xrange[*:*]\n");
    fprintf(gnuplot_spines,"set yrange[*:*] reverse\n");
    fprintf(gnuplot_spines,"set palette define (-1 'black', 1 'white')\n");
    fprintf(gnuplot_spines,"set cbrange [-1:1]\n");
}

void genera_imagenes_spines(int i_ejec, int i_temp) {
    fprintf(gnuplot_spines,"set output \"Configuracion_final_de_spines/Mapa_de_dominios_magneticos_%d/microestado_%d_ejecucion_%d.png\"\n", i_temp+1, i_temp+1, i_ejec);
    fprintf(gnuplot_spines,"plot \"Configuracion_final_de_spines/Mapa_de_dominios_magneticos_%d/microestado_%d_ejecucion_%d.dat\" matrix with image\n", i_temp+1, i_temp+1, i_ejec);
    fflush(gnuplot_spines);
}

void finaliza_imagenes_spines(void) {
    if (gnuplot_spines !=NULL) PCLOSE(gnuplot_spines);
}

void reporta_error_archivo(char *nombre_archivo) {
    fprintf(stderr, "Error en la escritura: No se pudo crear %s | Motivo: %s (errno=%d)\n", nombre_archivo, strerror(errno), errno);
    exit(EXIT_FAILURE);
}

void ejecuta_generador_imagenes_spines(void) {
    int i_ejec = 0, i_temp = 0;
    inicia_generador_imagenes_spines();
    for (i_ejec = ejec_ini; i_ejec <= ejec_fin; i_ejec++) {
        for (i_temp = 0; i_temp < num_temp; i_temp++) {
            genera_imagenes_spines(i_ejec, i_temp);
        }
    }
    finaliza_imagenes_spines();
}

void cadena_porcentaje(char caracter, int num) {
    if (num <= 0) return;
    putchar(caracter);
    cadena_porcentaje(caracter, num-1);
}

void imprime_avance_ejecucion(int i_ejec, int i_temp) {
    int lim_sup = ((i_temp + 1) * 10)/num_temp, lim_inf = (i_temp * 10)/num_temp, porcentaje;
    if (lim_sup == lim_inf) return;
    porcentaje = lim_sup * 10;

#pragma omp critical
    {
        printf("||\tHilo %d\t\t||\tEjecucion %d\t||\tAvance: %d%%", omp_get_thread_num(), i_ejec, porcentaje);
        printf("\t[");
        cadena_porcentaje('*', lim_sup);
        cadena_porcentaje('-', 10 - lim_sup);
        printf("]\t||\n");
    }
}