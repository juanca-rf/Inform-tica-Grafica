# OpenGL

Todo el código de este repositorio corresponde a las prácticas (acumuladas) de la asignatura Informática Gráfica. 

## Comandos básicos

A continuacion se recogen las teclas basicas para recorrer el funcionamiento del programa. 

#### Modos de Visualización

- 1 - Puntos 
- 2 - Lineas 
- 3 - Relleno 
- 4 - Ajedrez 
- 5 - Iluminación Plana
- 6 - Iluminación Gouraud
- 7 - Visión ortografica con alzado, planta y perfil
- 8 - Back buffer de la figura articulada

#### Objetos

- 'P' - Priámide
- 'C' - Cubo
- 'O' - Objeto PLY
- 'R' - Objeto por rotación (Peón)
- 'A' - Objeto articulado - Retro excavadora con entorno arbóreo 
- 'E' - Esfera
- 'M' - Modo manual del movimiento del objeto articulado
- '+' - Aumento de velocidad de movimiento del objeto articulado
- '-' - Disminución de velocidad de movimiento del objeto articulado

## Distribución de las prácticas

### 🏁Visualización

En la primera práctica se pidió la creación de modos de visualización, las cuales son:

- Puntos - Solo se visualizan los vertices de las figuras
- Lineas - Se visualizan las aristas de las figuras
- Relleno - Se visualizan las caras de la figura en un solo color
- Ajedrez - Las caras se visualizan con dos colores de forma parecida para el tablero de ajedrez

Además de la creación de un metodo de lectura de archivos en formato PLY.

### 🔃Rotación

Para las segunda practica el objetivo era conseguir un volumen cerrado a traves de la rotación de una linea de puntos (perfil). Se puede probar un ejemplo de su resultado pulsando la tecla 'R'. Se visualizará la figura de un peón del juego de ajedrez. 

Ésta se puede encontrar en el archivo *practicas_objetos_B5.cc*.

### 🏗️Figura articulada

Se pedía un objeto articulado el cual tuviera varios ejes de rotación con algunos de ellos encadenados. Pues la creación de una máquina retroexcavadora era lo más adecuado. 

Se puede controlar de forma manual cada articulación con las siguientes teclas:

- F1, F2 - Angulo de la base
- F3, F4 - Angulo de brazo interno
- F5, F6 - Angulo de brazo externo
- F7, F8 - Separación de palas
- F9, F10 - Angulo de giro de las palas

O se puede con la tecla *'M'* activar el movimiento automático el cual muestra los rangos máximos y mínimos de cada articulación. 

Se puede encontrar el desglose y grafo gerarquico de la figura en el pdf "Grafo jerárquico".

### 🔦Iluminación

En la práctica cuarta se debía poder visualizar las figuras con dos focos de iluminación, uno de ellos en movimiento.

Para la iluminación se ha tenido que calcular las normales a las caras y los vértices para representar el grado de intensidad de la luz colisionada con las superficies. Éstas superficies se han configurado independientemente su material, cambiando el tipo de reflejo y color para aumentar el realismo.

### 📍Selección

En la última práctica se añadió la posibilidad de seleccionar las piezas del objeto articulado.

Existen varios metodos de seleccion de piezas 3D pero en este caso se uso el metodo de buffer trasero. Donde se crea una imagen falsa de la figura donde cada pieza seleccionable tiene un color unico para así cuando el raton pulse sobre la figura real, se lea el color del buffer trasero para procesar la pieza de manera diferenciada. 