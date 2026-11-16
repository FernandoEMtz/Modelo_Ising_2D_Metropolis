# Simulación del modelo de Ising bidimensional con Metropolis

Codigo en modulos, parametros a editar para el funcionamiento:

      Parametros_c -> Constantes termodinamicas y constantes de control para Montecarlo 
      y la estadística. 
      
      Parametros_h -> Tamaño de la red y número de vecinos.

* Montecarlo usa el metodo de vecinos para obtener la aportación de energía por medio de un spin aleatorio, 
el metodo de vecinos está pensado para una red simple bidimensional, sin embargo, se puede hacer un cambio en esta parte
del codigo para implementar redes de diferentes estructuras, como por ejemplo una estructura
honey comb.


* Montecarlo usa el metodo de Metropolis para calcular la energía del sistema y su posterior cambio.


* Los números aleatorios se obtienen mediante la aplicación del metodo pcg32 en su modelo pcg_basic, esto para evitar
la remota, pero existente posibilidad de duplicar un numero aleatorio al obtener al mismo tiempo el valor del reloj.


* La aceptación en el metodo de Metropolis utiliza una lista de factores de Boltzmann para mejorar la eficiencia.


* La lista de factores de Boltzmann está pensada para funcionar con redes de diferentes formas, por lo tanto, diferente
cantidad de vecinos, para esta red se tiene el parametro con valor 4, este parametro se actualiza junto con el posterior
cambio en los vecinos, almacena la cantidad necesaria y se calcula en cada iteración.


* El cálculo del Hamiltoniano considera la aportación del campo magnetico con la variable h_campo, actualmente 
la variable h_campo se encuentra en 0.0


* El cálculo del Hamiltoniano también considera el cambio en la constante de acoplamiento físico del material
magnético está representada por la variable J, actualmente se encuentra en 1.0 siendo sumamente magnético.


* La aceptación del movimiento está condicionada a la interacción de los vecinos y del campo magnético, de forma que
la aceptación del cambio se determina con un spin y evita calcular nuevamente el sistema para compararlo con el
anterior logrando reducir enormemente la demanda de poder computacional.


* Se comunica con gnuplot para generar un mapa bidimensional de la configuracion final de los spines, pensado
para su aplicacion en Machine Learning, solo se genera la representacion de los spines en blanco y negro


* Se tiene un módulo encargado de la escritura de todos los datos estadisticos a fin de diferenciarlos de los
modulos para realizar los calculos físicos y estadisticos del ciclo de temperaturas y de Montecarlo.


* Dentro del módulo de escritura de datos también se generan imagenes con ayuda de Gnuplot, se tiene una bandera
que permite elegir si se quiere que se generen las imagenes o no.


* Los datos recopilados se organizan en 4 archivos diferentes:

      Datos generales -----> Recopila el reporte de cada ejecución realizada, permite encontrar anomalias
      Estadística general -> Recopila cada paso Montecarlo, la energía promedio, la magnetización promedio y la
                             aceptación del movimiento para cada cambio de estado en el sistema.
      Resultados generales-> Recopila la temperatura adimensional, energia promedio, susceptibilidad termica,
                             magnetización promedio y susceptibilidad magnetica.
      Mapa spin -----------> Genera la configuración final de la red de spines en un arreglo bidimensional
      Imagenes ------------> Se usa gnuplot para generar los archivos de imagen en conjunto con los arreglos
                             ya calculados, se ejecuta al terminar toda la ejecucion del archivo.

* Esta versión se encarga de generar varias simulaciones organizando los datos recopilados e indexados, esto para
permitir realizar promedios estadisticos y mejorar las aproximaciones obtenidas, además de facilitar la
posterior consulta si es que existe alguna anomalía en la simulación, por ejemplo un estancamiento en el cambio
de estados que genere un error en la simulación.