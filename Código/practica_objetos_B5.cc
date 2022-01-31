//**************************************************************************
// Práctica 5 usando objetos
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B5.h"

using namespace std;

// tipos
typedef enum
{
  CUBO,
  PIRAMIDE,
  OBJETO_PLY,
  ROTACION,
  ARTICULADO,
  ESFERA
} _tipo_objeto;
_tipo_objeto t_objeto = CUBO;
_modo modo = POINTS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_position;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=450,Window_high=450;


// objetos
_cubo cubo;
_piramide piramide(0.85, 1.3);
_objeto_ply  ply;
_rotacion rotacion;
_tanque tanque;
_Retro_Excavadora retro;
float velocidad_retro = 0.01;
_esfera esfera;

// Movimiento
float iterador = 0;
float velocidad = 0.001;
bool manual = true;
float giro_luz1 = 0;

//Vista plano
bool simple = 1;
bool front_only = 1;

// _objeto_ply *ply1;

int estadoRaton[3], xc, yc;

void pick_color(int x, int y);

//**************************************************************************
//
//***************************************************************************

void clean_window()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
  //  plano_delantero>0  plano_trasero>PlanoDelantero)
  glFrustum(-Size_x, Size_x, -Size_y, Size_y, Front_plane, Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

  // posicion del observador
  change_projection();
  glViewport(0,0,Window_width,Window_high);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -Observer_distance);
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{

  glDisable(GL_LIGHTING);
  glLineWidth(2);
  glBegin(GL_LINES);
  // eje X, color rojo
  glColor3f(1, 0, 0);
  glVertex3f(-AXIS_SIZE, 0, 0);
  glVertex3f(AXIS_SIZE, 0, 0);
  // eje Y, color verde
  glColor3f(0, 1, 0);
  glVertex3f(0, -AXIS_SIZE, 0);
  glVertex3f(0, AXIS_SIZE, 0);
  // eje Z, color azul
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, -AXIS_SIZE);
  glVertex3f(0, 0, AXIS_SIZE);
  glEnd();
}

//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

  switch (t_objeto)
  {
  case CUBO:
    cubo.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
    break;
  case PIRAMIDE:
    piramide.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
    break;
  case OBJETO_PLY:
    ply.draw(modo, 1.0, 0.6, 0.0, 0.0, 1.0, 0.3, 2);
    break;
  case ROTACION:
    rotacion.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
    break;
  case ESFERA:
    esfera.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
    break;
  case ARTICULADO:
    //tanque.draw(modo,0.5,0.7,0.2,0.3,0.6,0.3,2);
    retro.draw(modo, 0.89, 0.7, 0.0, 0.9, 0.6, 0.0, 2, velocidad_retro, simple);
    break;
  }
}

void luces()
{
  float luz1[] = {1, 1, 1, 1},
        pos1[] = {200, 20, 0, 1},
        luz2[] = {0, 1, 1, 1},
        pos2[] = {-50, 0, 0, 1}, amb[] = {0.2, 0.2, 0.2, 1};

  glLightfv(GL_LIGHT1, GL_DIFFUSE, luz1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, luz1); // brillo

  glLightfv(GL_LIGHT2, GL_DIFFUSE, luz2);
  glLightfv(GL_LIGHT2, GL_SPECULAR, luz2); // brillo

  glPushMatrix();
  glRotatef(giro_luz1, 0, 1, 0);
  glLightfv(GL_LIGHT1, GL_POSITION, pos1);
  glPopMatrix();
  glLightfv(GL_LIGHT2, GL_POSITION, pos2);

  glDisable(GL_LIGHT0);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
  glEnable(GL_LIGHT1);
  // glEnable (GL_LIGHT2);
}

//**************************************************************************
//
//***************************************************************************

void draw(void)
{
  int Alto = Window_high;
  int Ancho = Window_width;

  if(simple){
    glDrawBuffer(GL_FRONT);
    clean_window();
    change_observer();
    luces();
    draw_axis();
    draw_objects();

    if (t_objeto == ARTICULADO)
    {
      glDrawBuffer(GL_BACK);
      clean_window();
      change_observer();
      retro.seleccion(); // Funcion para pintar la selección en el background MIRAR FOTO DIA 10-12-21
    }
  }else{
    glDrawBuffer(GL_FRONT);
    clean_window();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(Observer_distance*-1.0,Observer_distance*1.0,Observer_distance*-1.0,Observer_distance*1.0,-100.0,100.0);
    glViewport(0,Alto/2,Ancho/2,Alto/2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(Observer_distance*-1.0,Observer_distance*1.0,Observer_distance*-1.0,Observer_distance*1.0,-100.0,100.0);
    glViewport(Ancho/2,Alto/2,Ancho/2,Alto/2);
    glRotatef(-90,0,1,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(Observer_distance*-1.0,Observer_distance*1.0,Observer_distance*-1.0,Observer_distance*1.0,-100.0,100.0);
    glViewport(0,0,Ancho/2,Alto/2);
    glRotatef(90,1,0,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(Observer_distance*-1.0,Observer_distance*1.0,Observer_distance*-1.0,Observer_distance*1.0,-100.0,100.0);
    glViewport(Ancho/2,0,Ancho/2,Alto/2);
    glRotatef(-45,0,1,0);
    glRotatef(-22.5,0,0,1);
    glRotatef(22.5,1,0,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();
    if (t_objeto == ARTICULADO)
    { 
      if(front_only)
        glDrawBuffer(GL_BACK);
      else
        glDrawBuffer(GL_FRONT_AND_BACK);
      clean_window();
      
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(Observer_distance*-1.0,Observer_distance*1.0,Observer_distance*-1.0,Observer_distance*1.0,-100.0,100.0);
      glViewport(0,Alto/2,Ancho/2,Alto/2);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      retro.seleccion();

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(Observer_distance*-1.0,Observer_distance*1.0,Observer_distance*-1.0,Observer_distance*1.0,-100.0,100.0);
      glViewport(Ancho/2,Alto/2,Ancho/2,Alto/2);
      glRotatef(-90,0,1,0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      retro.seleccion();

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(Observer_distance*-1.0,Observer_distance*1.0,Observer_distance*-1.0,Observer_distance*1.0,-100.0,100.0);
      glViewport(0,0,Ancho/2,Alto/2);
      glRotatef(90,1,0,0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      retro.seleccion();

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(Observer_distance*-1.0,Observer_distance*1.0,Observer_distance*-1.0,Observer_distance*1.0,-100.0,100.0);
      glViewport(Ancho/2,0,Ancho/2,Alto/2);
      glRotatef(-45,0,1,0);
      glRotatef(-22.5,0,0,1);
      glRotatef(22.5,1,0,0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      retro.seleccion();
    }

  }

  glFlush();
}

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1, int Alto1)
{
  float Aspect_ratio;

  Aspect_ratio = (float)Alto1 / (float)Ancho1;
  Size_y = Size_x * Aspect_ratio;
  change_projection();
  glViewport(0, 0, Ancho1, Alto1);
  glutPostRedisplay();
}

//**********-o*****************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1, int x, int y)
{
  switch (toupper(Tecla1))
  {
  case 'Q':
    exit(0);
  case '1':
    modo = POINTS;
    break;
  case '2':
    modo = EDGES;
    break;
  case '3':
    modo = SOLID;
    break;
  case '4':
    modo = SOLID_CHESS;
    break;
  case '5':
    modo = SOLID_ILLUMINATED_FLAT;
    break;
  case '6':
    modo = SOLID_ILLUMINATED_GOURAUD;
    break;
  case 'P':
    t_objeto = PIRAMIDE;
    break;
  case 'C':
    t_objeto = CUBO;
    break;
  case 'O':
    t_objeto = OBJETO_PLY;
    break;
  case 'R':
    t_objeto = ROTACION;
    break;
  case 'A':
    t_objeto = ARTICULADO;
    break;
  case 'E':
    t_objeto = ESFERA;
    break;
  case 'M':
    manual = !manual;
    break;
  case '+':
    velocidad_retro += 0.1;
    break;
  case '-':
    velocidad_retro -= 0.1;
    break;
  case '7':
    simple=!simple;
  break;
  case '8':
    front_only=!front_only;
  break;
  }
  glutPostRedisplay();
}

//***************************************************************************
// Funcion l-olamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1, int x, int y)
{

  switch (Tecla1)
  {
  case GLUT_KEY_LEFT:
    Observer_angle_y--;
    break;

  case GLUT_KEY_RIGHT:
    Observer_angle_y++;
    break;

  case GLUT_KEY_UP:
    Observer_angle_x--;
    break;

  case GLUT_KEY_DOWN:
    Observer_angle_x++;
    break;

  case GLUT_KEY_PAGE_UP:
    Observer_distance *= 1.2;
    break;

  case GLUT_KEY_PAGE_DOWN:
    Observer_distance /= 1.2;
    break;

  case GLUT_KEY_F1:
    if (!manual)
    {
      velocidad -= 0.0001;
      cout << "Velocidad " << velocidad << endl;
    }
    else
      retro.giro_base += 5;
    break;

  case GLUT_KEY_F2:
    if (!manual)
    {
      velocidad += 0.0001;
      cout << "Velocidad " << velocidad << endl;
    }
    else
      retro.giro_base -= 5;
    break;

  case GLUT_KEY_F3:
    if (retro.brazo.giro_brazo1 < retro.brazo.giro1_max)
      retro.brazo.giro_brazo1 += 5;
    break;

  case GLUT_KEY_F4:
    if (retro.brazo.giro_brazo1 > retro.brazo.giro1_min)
      retro.brazo.giro_brazo1 -= 5;
    break;

  case GLUT_KEY_F5:
    if (retro.brazo.giro_brazo2 < retro.brazo.giro2_max)
      retro.brazo.giro_brazo2 += 5;
    break;

  case GLUT_KEY_F6:
    if (retro.brazo.giro_brazo2 > retro.brazo.giro2_min)
      retro.brazo.giro_brazo2 -= 5;
    break;

  case GLUT_KEY_F7:
    if (retro.brazo.separacion > 0)
      retro.brazo.separacion -= 0.1;
    break;

  case GLUT_KEY_F8:
    if (retro.brazo.separacion < retro.brazo.separacion_max)
      retro.brazo.separacion += 0.1;
    break;

  case GLUT_KEY_F9:
    retro.brazo.giro_mano += 5;
    break;

  case GLUT_KEY_F10:
    retro.brazo.giro_mano -= 5;
    break;

  case GLUT_KEY_F11:
    retro.posz += velocidad_retro;
    break;

  case GLUT_KEY_F12:
    if (retro.posz > -50)
      retro.posz -= velocidad_retro;

    break;

  case GLUT_KEY_HOME:
    retro.posx += velocidad_retro;
    Observer_position = retro.posx;
    break;
  case GLUT_KEY_END:
    retro.posx -= velocidad_retro;
    Observer_position = retro.posx;
    break;
  }
  glutPostRedisplay();
}

//***************************************************************************
// Funciones para manejo de eventos del ratón
//***************************************************************************

void clickRaton(int boton, int estado, int x, int y)
{
  if (boton == GLUT_RIGHT_BUTTON)
  {
    if (estado == GLUT_DOWN)
    {
      estadoRaton[2] = 1;
      xc = x;
      yc = y;
    }
    else
      estadoRaton[2] = 1;
  }
  if (boton == GLUT_LEFT_BUTTON)
  {
    if (estado == GLUT_DOWN)
    {
      estadoRaton[2] = 2;
      xc = x;
      yc = y;
      pick_color(xc, yc);
    }
  }

  //Zoom con rueda de raton https://stackoverflow.com/questions/14378/using-the-mouse-scrollwheel-in-glut
  if( boton == 3 )
    Observer_distance-=0.4;

  if( boton == 4 )
    Observer_distance+=0.4;
}

/*************************************************************************/

void getCamara(GLfloat *x, GLfloat *y)
{
  *x = Observer_angle_x;
  *y = Observer_angle_y;
}

/*************************************************************************/

void setCamara(GLfloat x, GLfloat y)
{
  Observer_angle_x = x;
  Observer_angle_y = y;
}

/*************************************************************************/

void RatonMovido(int x, int y)
{
  float x0, y0, xn, yn;
  if (estadoRaton[2] == 1)
  {
    getCamara(&x0, &y0);
    yn = y0 + (x - xc);
    xn = x0 + (y - yc);
    setCamara(xn, yn);
    xc = x;
    yc = y;

    glutPostRedisplay();
  }
}

void procesar_color(unsigned char color[3])
{
  int i;

  //for (i = 0; i < tanque.piezas; i++)
  for (i = 0; i < retro.piezas; i++)
  {
    //if (color[0] == tanque.color_selec[0][i])
    if (color[0] == retro.color_selec[0][i])
    {
      // tanque.activo[i] = !tanque.activo[i];
      // tanque.cambio[i] = !tanque.cambio[i];
      retro.activo[i] = !retro.activo[i];
      retro.cambio[i] = !retro.cambio[i];
      glutPostRedisplay();
    }
  }
}

void pick_color(int x, int y) // Posicion del raton x e y
{
  GLint viewport[4];
  unsigned char pixel[3];

  glGetIntegerv(GL_VIEWPORT, viewport);
  glReadBuffer(GL_BACK);
  glReadPixels(x, Window_high - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte *)&pixel[0]);
  printf(" valor x %d, valor y %d, color %d, %d, %d \n", x, y, pixel[0], pixel[1], pixel[2]);

  procesar_color(pixel);
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{

  // se inicalizan la ventana y los planos de corte
  Size_x = 0.5;
  Size_y = 0.5;
  Front_plane = 1;
  Back_plane = 1000;

  // se incia la posicion del observador, en el eje z
  Observer_distance = 4 * Front_plane;
  Observer_angle_x = 0;
  Observer_angle_y = 0;

  // se indica cua*ply1l sera el color para limpiar la ventana	(r,v,a,al)
  // blanco=(1,1,1,1) rojo=(1,0,0,1), ...
  glClearColor(1, 1, 1, 1);

  // se habilita el z-bufer
  glEnable(GL_DEPTH_TEST);
  change_projection();
  glViewport(0, 0, Window_width, Window_high);
}

void movimiento()
{
        if (!manual)
        {
                retro.giro_base = sin(iterador) * 360;
                retro.brazo.giro_brazo1 = sin(iterador) * 30 + 45;
                retro.brazo.giro_brazo2 = sin(iterador) * 45 - 45;
                retro.brazo.giro_mano = sin(iterador * 5) * 360;
                retro.brazo.separacion = sin(iterador);
                iterador += velocidad;
        }
        giro_luz1 += 0.3;
        glutPostRedisplay();
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv)
{

  // creación del objeto ply

  ply.parametros(argv[1]);

  // perfil

  vector<_vertex3f> perfil2;
  _vertex3f aux;
  aux.x = 1.0;
  aux.y = -1.4;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 1.0;
  aux.y = -1.1;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.5;
  aux.y = -0.7;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.4;
  aux.y = -0.4;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.4;
  aux.y = 0.5;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.5;
  aux.y = 0.6;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.3;
  aux.y = 0.6;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.5;
  aux.y = 0.8;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.55;
  aux.y = 1.0;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.5;
  aux.y = 1.2;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.3;
  aux.y = 1.4;
  aux.z = 0.0;
  perfil2.push_back(aux);
  rotacion.parametros(perfil2, 6, 1);

  // se llama a la inicialización de glut
  glutInit(&argc, argv);

  // se indica las caracteristicas que se desean para la visualización con OpenGL
  // Las posibilidades son:
  // GLUT_SIMPLE -> memoria de imagen simple
  // GLUT_DOUBLE -> memoria de imagen doble
  // GLUT_INDEX -> memoria de imagen con color indizado
  // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
  // GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
  // GLUT_DEPTH -> memoria de profundidad o z-bufer
  // GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  // posicion de la esquina inferior izquierdad de la ventana
  glutInitWindowPosition(Window_x, Window_y);

  // tamaño de la ventana (ancho y alto)
  glutInitWindowSize(Window_width, Window_high);

  // llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
  // al bucle de eventos)
  glutCreateWindow("PRACTICA - 5");

  // asignación de la funcion llamada "dibujar" al evento de dibujo
  glutDisplayFunc(draw);
  // asignación de la funcion llamada "change_window_size" al evento correspondiente
  glutReshapeFunc(change_window_size);
  // asignación de la funcion llamada "normal_key" al evento correspondiente
  glutKeyboardFunc(normal_key);
  // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
  glutSpecialFunc(special_key);

  glutIdleFunc(movimiento);

  // eventos ratón
  glutMouseFunc(clickRaton);
  glutMotionFunc(RatonMovido);

  // funcion de inicialización
  initialize();

  // inicio del bucle de eventos
  glutMainLoop();
  return 0;
}
