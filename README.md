# Simulación del modelo de Ising bidimensional con Metropolis en C.
Modelo paralelizado con OpenMP, uso de tablas pre calculadas y validación con datos teóricos externos.

# ¿Qué hace el código?
El código genera una red de partículas en dos dimensiones de trama cuadrada donde cada partícula puede presentar un spin up o spin down, la interacción local permite que los spines tiendan a alinearse a temperaturas bajas, por el contrario, el sistema tiende al desorden entre más aumenta la energía térmica del sistema.

Este modelo se estructura con condiciones de frontera periódicas, por lo que la interacción entre vecinos se mantiene en toda la malla, en el caso de los spines que se encuentran en los extremos, estos interactúan con los primeros o los últimos espines de las columnas o de las columnas dependiendo sea el caso. Tambien permite incluir el efecto de un campo magnético externo, así como variar la constante de acoplamiento, aumentando o reduciendo el costo de energía para la interacción entre vecinos.

El proyecto calcula la energía interna por spin, la magnetización absoluta por spin, el calor específico por spin y la susceptibilidad magnética por spin, además de generar un mapa de la configuración final de los spines pensados para utilizarse en entrenamiento de Machine Learning.

# Estructura del proyecto
El proyecto está construido de manera modular con el objetivo de poder modificar o añadir funciones dependiendo la necesidad del proyecto. Por ejemplo, cambiar o modificar la red bidimensional de trama cuadrada propuesta y con ello también el módulo de vecinos que utiliza el algoritmo de metropolis sin cambiar todo el algoritmo.
Hay que tener especial cuidado con redes de un rango menor a 3 por cada lado, ya que esto puede dar problemas con la ejecución.

El generador de números aleatorios utilizado es la versión básica de PCG32 desarrollada por Melissa O’Neil, ya que su generador tiene buena calidad estadística, es rápido, pequeño, además de trabajar con una semilla que permite reproducibilidad.   

Se utiliza paralelización mediante OpenMP para reducir el tiempo de cómputo, cada hilo ejecuta una simulación independiente, por lo que cuenta con su propia red y su generador de números pseudoaleatorios PCG32 y flujo propio determinado por el número de ejecución

Para hacer más eficiente la simulación se precalcula una tabla de Boltzmann para cada temperatura, que, a palabras simples, permite consultar el valor generado por el cambio de energía interna que presentan diferentes configuraciones de interacción entre vecinos, pasando de un cálculo usando exponenciales a una consulta, ya que su interacción no presenta cambios debido a que solo pueden presentar 5 valores posibles de la suma de vecinos siendo (-4, -2, 0, 2, 4).

La simulación se realiza con una red aleatoria (sin recocido), el promedio entre ejecuciones se realiza fuera del repositorio y la termalización del sistema es fija en todas las temperaturas. Siendo la susceptibilidad magnética calculada mediante: (N <m^2> - <|m|>^2/T)


Los resultados de la simulación se han validado con el resultado exacto de Onsager verificando a la energía por spin (E/N) y la magenetizacion absoluta |M| a T = 2.0 (-1.7447 y 0.9108 comparados con -1.7455 y 0.9113 respectivamente usando 6,000 de 12,000 pasos para realizar la estadística) y se adjunta el archivo con los datos que se usaron para comparar en el archivo con directorio:

    data/reference/onsager_exacto.dat

# Compilación

Para poder ejecutar el codigo se necesita un compilador con OpenMP, CMake en una version mayor o igual a la 3.20 y gnuplot para la generación de los mapas, si prescinde de los mapas, gnuplot es opcional, solo hay que asegurar usar la bandera de genera_mapas siempre en false.
Si se quiere ejecutar por medio de la terminal, se sugiere el siguiente comando:
     
    gcc -O2 -Wall -Wextra -fopenmp -Iinclude -Ithird_party/pcg-c-basic-0.9 main.c src/gestor_de_datos_de_salida.c src/gestor_de_ejecuciones.c src/gestor_de_red.c src/gestor_de_observables.c src/gestor_de_parametros.c src/gestor_metropolis.c src/gestor_tabla_boltzmann.c third_party/pcg-c-basic-0.9/pcg_basic.c -o Ising_M.exe -lm


* Los campos que modificar para realizar la simulación se encuentran en gestor_de_parametros.c y en su cabecera


    /* ---- *************************************** Dimensiones del sistema **************************************** ---- */
    #define N_X     40                      //Número de columnas en la red
    #define N_Y     40                      //Número de filas    en la red
    #define N_SP    (N_X*N_Y)               //Número total de espines en la red bidimensional
    #define N_VEC   4                       //Número de vecinos, en una red bidimensional son 4 vecinos
    //Se maneja la variable para redes más complejas para las tablas de Boltzmann
    

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

# Salida de archivos y estadística

* Los datos recopilados se organizan en 4 directorios diferentes y se escriben en la carpeta raíz donde se esté ejecutando, se recomienda ejecutar en una carpeta aparte:
Por su parte, las imágenes se almacenan también en la carpeta del mapa de los dominios magnéticos   

      * Reportes_de_simulaciones --------> Recopila el reporte de cada ejecución realizada, permite encontrar anomalías.
      
      * Estadistica_por_paso_Montecarlo -> Recopila cada paso Montecarlo, la energía promedio, la magnetización promedio y la
                                           tasa de aceptación por paso.
      
      * Resultados_finales---------------> Recopila la temperatura adimensional, energía promedio, susceptibilidad termica,
                                           magnetización promedio y susceptibilidad magnética.

      * Configuracion_final_de_spines ---> Genera la configuración final de la red de spines en un arreglo bidimensional
      
      * Imagenes ------------------------> Se usa gnuplot para generar los archivos de imagen en conjunto con los arreglos
                                           ya calculados, se ejecuta al terminar toda la ejecucion del archivo.

Los datos escritos en resultado_ejecucion_N.dat están organizados de esta manera:

    Temperatura | Energía por spin | Susceptibilidad térmica | susceptibilidad magnética 

Se gestionan los datos escritos de manera indexada por temperatura, ejecución y simulación realizada, esto para permitir realizar su tratamiento para realizar promedios estadísticos y mejorar las aproximaciones obtenidas, además de facilitar la consulta si es que existe alguna anomalía, como, por ejemplo, un estancamiento en el cambio de estados que genere un error en la simulación.

Por su parte, las imágenes se generan a través de gnuplot, se puede modificar la bandera de generación para elegir si se generan o no, ya que esto reduce la cantidad de datos de salida del proyecto.