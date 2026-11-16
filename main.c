#include <inttypes.h>
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#include "include/gestor_de_datos_de_salida.h"
#include "include/gestor_de_ejecuciones.h"
#include "include/gestor_de_parametros.h"
#include "include/gestor_tabla_boltzmann.h"

int main(void) {
    int i_ejec = 0;
    int num_ejec = ejec_fin - ejec_ini +1;
    int tot_hilos_disp = omp_get_max_threads();
    int tot_hilos_usar = ( num_ejec <= num_hilos_max) ? num_ejec : num_hilos_max;

    omp_set_num_threads(tot_hilos_usar);

    //Pre-generación de las carpetas para que la escritura paralela no tenga problemas de ejecucion
    crea_carpetas_salida();

    printf("\n\n  ============================= Modelo de Ising paralelizado =========================== \n");
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\tTotal de ejecuciones a realizar:\t%d\t\t\t||\n", num_ejec);
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\tSemilla usada para las ejecuciones:\t%" PRIu64 "\t\t||\n", semilla);
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\tNumero de hilos a utilizar durante la ejecucion %d de %d (capado a %d de %d)\t||\n",tot_hilos_usar, num_hilos_max, num_hilos_max, tot_hilos_disp);
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("  ======================================================================================\n");
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\t\tCalculando tablas de Boltzmann\t\t\t\t||\n");
    //Pre-generacion tablas Boltzmann para evitar recalculos innecesarios
    //tabla_boltzmann_t tablas_boltzmann[num_temp+1];
    tabla_boltzmann_t *tablas_boltzmann = malloc((size_t)(num_temp) * sizeof(*tablas_boltzmann));
    if (tablas_boltzmann == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para las tablas Boltzmann\n");
        return EXIT_FAILURE;
    }
    pre_calcula_tabla_boltzmann(tablas_boltzmann);
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\t\tTablas de Boltzmann calculadas\t\t\t\t||\n");
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\tTotal de tablas calculadas (%d x %d entradas): \t%d\t\t||\n", num_temp, tablas_boltzmann[0].tam, num_temp * tablas_boltzmann[0].tam);
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("  ======================================================================================\n");
    //Paralelizacion
    #pragma omp parallel for
    for (i_ejec = ejec_ini; i_ejec <= ejec_fin; ++i_ejec) {
        recorrido_de_temperaturas(tablas_boltzmann, i_ejec);
    }
    libera_memoria_tabla_temperatura_boltzmann(tablas_boltzmann);
    free(tablas_boltzmann);
    tablas_boltzmann = NULL;
    printf("  ======================================================================================\n");
    if (genera_mapas) {
        printf("||\t\t\t\tInicio de generacion de imagenes\t\t\t||\n");
        printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
        ejecuta_generador_imagenes_spines();
        printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
        printf("||\t\t\t\tImagenes generadas con exito\t\t\t\t||\n");
        printf("  ======================================================================================\n");
    }
    return 0;
}

//gcc -O2 -Wall -Wextra -fopenmp -Iinclude -Ithird_party/pcg-c-basic-0.9 main.c src/gestor_de_datos_de_salida.c src/gestor_de_ejecuciones.c src/gestor_de_red.c src/gestor_de_observables.c src/gestor_de_parametros.c src/gestor_metropolis.c src/gestor_tabla_boltzmann.c third_party/pcg-c-basic-0.9/pcg_basic.c -o ejecuta_V4.exe -lm
