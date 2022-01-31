//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.hpp"

using namespace std;

const float AXIS_SIZE=5000;
typedef enum{
       POINTS,
       EDGES,
       SOLID_CHESS,
       SOLID,
       SOLID_ILLUMINATED_FLAT, 
       SOLID_ILLUMINATED_GOURAUD,
       SELECT
       } _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void   draw_solido(float r, float g, float b);
void 	draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2);
void   draw_seleccion(int r, int g, int b); //Metodo nuevo para esta practica, ¡¡¡¡hay que hacerla en base a la 4!!!
void 	draw_iluminacion_plana( );
void 	draw_iluminacion_suave( );

void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

void	calcular_normales_caras(int inv);
void 	calcular_normales_vertices();

vector<_vertex3i> caras;
vector<_vertex3f> normales_caras;
vector<_vertex3f> normales_vertices;

bool b_normales_caras;
bool b_normales_vertices;

_vertex4f ambiente_difusa;     //coeficientes ambiente y difuso
_vertex4f especular;           //coeficiente especular
float brillo;                  //exponente del brillo 

};


//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=0.75);
};

//*************************************************************************
// clase cabezal
//*************************************************************************

class _triangulo: public _triangulos3D
{
public:
	_triangulo(float alt=2, float anch=2, float prof=1);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

int   parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion: public _triangulos3D
{
public:
       _rotacion();
void  parametros(vector<_vertex3f> perfil1, int num1, int tapas);
void parametros(vector<_vertex3f> perfil, int num, char eje);

vector<_vertex3f> perfil; 
int num;
};


// *************************************************************************
// clase cono
//*************************************************************************
class _cono: public _triangulos3D
{
public:
	_cono( float diametro, float altura, char eje);
};

class _esfera: public _triangulos3D
{
public:
       _rotacion esfera;
	_esfera(float diametro=1, int precision=64, int inv=0);
void       draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);
};

/**
 * @brief Clase arbol
 * 
 */
class _arbol: public _triangulos3D
{
public:
       _cono *copa1, *copa2;
       _rotacion tronco;
       float verde1=0.7, verde2=0.5; //copas
       float rojo=0.507, verde3=0.23;//tronco
       float altura;
       _arbol( float diametro, float altura);
       void 	draw(_modo modo, float grosor);
};

// *************************************************************************
// clase entorno
//*************************************************************************
class _entorno: public _triangulos3D
{
public:
       _arbol *arbol;
       int tamx=50,tamz=8;
       int **posiciones;
       float verde1=0.7, verde2=0.5; //copas
       float rojo=0.507, verde3=0.23;//tronco
       _cubo  suelo;
       _cubo  cielo;
       _esfera cielo_esfera;


       _entorno();
       void 	draw(_modo modo, float grosor);
};


//************************************************************************
// objeto articulado: tanque
//************************************************************************

class _chasis: public _triangulos3D
{
public:
       _chasis();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;

protected:
_rotacion  rodamiento;
_cubo  base;
};

//************************************************************************

class _torreta: public _triangulos3D
{
public:
       _torreta();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;
float anchura;

protected:
_cubo  base;
_piramide parte_trasera;
};

//************************************************************************

class _tubo: public _triangulos3D
{
public:
       _tubo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_rotacion tubo_abierto; // caña del cañón
};

//************************************************************************

class _tanque: public _triangulos3D
{
public:
       _tanque();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);
void    seleccion();

float giro_tubo;
float giro_torreta;

float giro_tubo_min;
float giro_tubo_max;

float  color_pick[3]; //Color rgb para colorear la pieza seleccionada
int    color_selec[3][3]; //Color para CADA SELECCION 3 colores(chasis torreta tubo) 3 piezas
int    activo[3]; //3 piezas 
int    cambio[3];
int    piezas; //guardar el numero de piezas que tengo

protected:
_chasis  chasis;
_torreta  torreta;
_tubo     tubo;
};


//**************************************************************************
// Objeto Articulado : Retro_Excavadora
//**************************************************************************

/**
 * @brief Chasis de la retro excavadora
 * 
 */
class _Retro_Excavadora_chasis: public _triangulos3D
{
public:
       _Retro_Excavadora_chasis();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor, float velocidad);

protected:
const float angulo_avance = (360)/(2*M_PI*0.5);//(360*velocidad_avance)/(2*M_PI);  Surge de despejar la ecuacion del calculo de longitud de un arco : Lngitud_avance = 2*PI*Radio*angulo/360
_rotacion  rodamiento;
_rotacion  rodamiento1;
_cubo  base;
};

/**
 * @brief Cuerpo y cabina de la excavadora
 * 
 */
class _Retro_Excavadora_cuerpo: public _triangulos3D
{
public:
       _Retro_Excavadora_cuerpo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_cubo  cuerpo;
_cubo  cabina, cabina1;
_rotacion  rodamiento;

};

class _Retro_Excavadora_brazo: public _triangulos3D
{
public:
       _Retro_Excavadora_brazo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);
_cubo brazos;
_triangulo cabezal;
_rotacion rodamiento;
_rotacion rodamiento1;

float giro_brazo1;
float giro_brazo2;
float giro_mano;
float separacion;
float negro_1 = 0.2;
float negro_2 = 0.5;

float giro1_min = 0;
float giro1_max = 90;
float giro2_min = -90;
float giro2_max = 0;
float separacion_max = 1;
};


//Clase general para Retro_Excavadora
class _Retro_Excavadora: public _triangulos3D
{
public:
       _Retro_Excavadora();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor, float velocidad, bool paisaje=1);
void seleccion();

float giro_base;
float posx, posz;
float negro_1 = 0.2;
float negro_2 = 0.5;

float  color_pick[3]; //Color rgb para colorear la pieza seleccionada
int    color_selec[3][3]; //Color para CADA SELECCION 
int    activo[3]; //3 piezas 
int    cambio[3];
int    piezas = 3; //guardar el numero de piezas que tengo

_Retro_Excavadora_chasis  chasis;
_Retro_Excavadora_cuerpo cuerpo;
_Retro_Excavadora_brazo brazo;
_entorno *entorno;
};

