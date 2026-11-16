//
// Created by Fernando on 14/09/2026.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gestor_de_parametros.h"
#include "gestor_tabla_boltzmann.h"

/* Una tabla de factores de Boltzmann obtiene y almacena el cálculo de la exp(-DeltaE/T) de la energía para el metodo
 * de Metropolis, esto debido a que por el metodo de vecinos para la red simple bidimensional solo tenemos 5 posibles
 * valores, siendo -4, -2, 0, 2 y 4.
 * De modo que al solo consultar los datos nos ahorramos tiempo de ejecucion al no calcular cada valor por cada
 * interaccion realizada, reduciendo enormemente el tiempo de cada simulación, para evitar el caso de que el metodo
 * solo funcione para la red bidimensional simple, se propone mantener la aportacion de vecinos por separado, en caso
 * de llegar a aplicar algún otro tipo de configuracion para la red, por ello la tabla se calcula en cada ejecucion y
 * no solo se deja como un valor ya pre calculado.
 *
 * Aclarando esto, el cambio de la energía se obtiene mediante
 * Delta E = 2 * J * s_i * Metodo de Vecinos
 *
 * También se tiene considerada la implicación del campo magnetico, por si se requiere hacer más compleja la simulacion
 * teniendo entonces:
 *
 * Delta E = 2 * s_i * (J * Metodo de vecinos + campo magnetico)
 *
 * Por lo que: q = J * (suma_spin_vecinos + h_campo) -> Delta E = 2 * q * s_i
 * La tabla guarda a exp(2*q*beta) -> exp(-2*q/T)
 * siendo que beta = 1/temp_star la dejamos implicita para comodidad
 *
 * El arreglo siempre tiene un tamaño de n_vec + 1, se hace dinamico para que se vaya llenando por su cuenta, aunque
 * sepamos el tamaño que puede tener.
 *
 * IMPORTANTE: La lista puede tener problemas con redes de tamaño n_x o n_y = 2, por considerar más de una vez a un
 * spin como vecino de otro.
*/

tabla_boltzmann_t calculo_tabla_boltzmann(double temp_star) {
    tabla_boltzmann_t tabla;
    int i, suma_spin_vecinos;
    double q;

    tabla.tam = N_VEC+1;
    tabla.valores = malloc(tabla.tam * sizeof(*tabla.valores));

    if (tabla.valores == NULL) {
        fprintf(stderr,"Error al guardar memoria para %d entradas\n", tabla.tam);
        exit(EXIT_FAILURE);
    }

    //Calculamos los valores para la tabla, recordemos que usamos el puntero 0, por eso es menor que tabla.tam
    for (i = 0; i < tabla.tam; i++) {
        suma_spin_vecinos = -N_VEC + 2*i;
        q = J_intns_aco * (double)suma_spin_vecinos + h_cpo_mgntc_exter;   //Delta E queda implicito
        tabla.valores[i] = exp(-2.0*q/temp_star);//beta queda implicito
    }
    return tabla;
}
/* Ya que calculamos el valor de exp(-DeltaE/T)
 * tenemos suma_vecinos con h_vec sin multiplicar por el spin, tenemos q = J*h_vec + h_campo
 * Delta E = 2 * spin_actual * q
 * Cuando tenemos spin positivo es exp(-DeltaE/T) -> exp(-2q/T)
 * Cuando tenemos spin negativo es exp(-DeltaE/T) -> exp(+2q/T) = 1/tabla[i]
 * Profundizar más en explicar esto, ya que descartamos parte de metropolis
 */
double consulta_tabla_boltzmann(const tabla_boltzmann_t *tabla, int suma_spin_vecinos, int spin_actual) {
    int i = (suma_spin_vecinos + N_VEC)/2;
    assert(i >= 0 && i < tabla->tam);
    return (spin_actual == 1) ? tabla->valores[i] : 1.0/tabla->valores[i];
}

void libera_memoria_tabla_boltzmann(tabla_boltzmann_t *tabla) {
    free(tabla->valores);
    tabla->valores = NULL;
    tabla->tam = 0;
}

//Generamos una tabla de rango n_temp que apunte a las 5 posiciones de la tabla
void pre_calcula_tabla_boltzmann(tabla_boltzmann_t *tablas_boltzmann) {
    int i_temp;
    double temp_star;

    for (i_temp = 0; i_temp < num_temp; i_temp++) {
        temp_star = calcula_temp_star(i_temp);
        tablas_boltzmann[i_temp] = calculo_tabla_boltzmann(temp_star);
    }
}
void libera_memoria_tabla_temperatura_boltzmann(tabla_boltzmann_t *tablas_boltzmann) {
    int i_temp;
    for (i_temp = 0; i_temp < num_temp; i_temp++) {
        libera_memoria_tabla_boltzmann(&tablas_boltzmann[i_temp]);
    }
}