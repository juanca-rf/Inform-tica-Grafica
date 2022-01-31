//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include "objetos_B5.h"
#include <random>

/**
 * @brief Random for int numbers
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int random(int min, int max) // range : [min, max]
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL)); // seeding for the first time only!
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
	//**** usando vertex_array ****
	glPointSize(grosor);
	glColor3f(r, g, b);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawArrays(GL_POINTS, 0, vertices.size());

}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
	b_normales_caras = false;
	b_normales_vertices = false;

	ambiente_difusa = _vertex4f(0.9, 0.4, 0.4, 1.0); // coeficientes ambiente y difuso
	especular = _vertex4f(0.5, 0.5, 0.5, 1.0);		 // coeficiente especular
	brillo = 120;									 // exponente del brillo
}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
	//**** usando vertex_array ****
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(grosor);
	glColor3f(r, g, b);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_TRIANGLES, caras.size() * 3, GL_UNSIGNED_INT, &caras[0]);

}

//*************************************************************************
// dibujar en modo sólido
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
	int i;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// dibujar en modo sólido con apariencia de ajedrez
//*************************************************************************

void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2)
{
	int i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		if (i % 2 == 0)
			glColor3f(r1, g1, b1);
		else
			glColor3f(r2, g2, b2);
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// dibujar en modo iluminación plano
//*************************************************************************

void _triangulos3D::draw_iluminacion_plana()
{
	int i;
	if (b_normales_caras == false)
		calcular_normales_caras(0);

	glEnable(GL_LIGHTING);
	glShadeModel(GL_FLAT);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&ambiente_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glNormal3fv((GLfloat *)&normales_caras[i]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();

	glDisable(GL_LIGHTING);
}

//*************************************************************************
// calcular normales a caras
//*************************************************************************
void _triangulos3D::calcular_normales_caras(int inv)
{
	_vertex3f a1, a2, n;
	normales_caras.resize(caras.size());

	for (int i = 0; i < caras.size(); i++)
	{
		// obtener dos vectores en el triángulo y calcular el producto vectorial
		a1 = vertices[caras[i]._1] - vertices[caras[i]._0];
		a2 = vertices[caras[i]._2] - vertices[caras[i]._0];
		if (inv == 1)
			n = a2.cross_product(a1);
		else
			n = a1.cross_product(a2);
		// modulo
		float m = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
		// normalización
		normales_caras[i] = _vertex3f(n.x / m, n.y / m, n.z / m);
	}

	b_normales_caras = true;
}

void _triangulos3D::draw_iluminacion_suave()
{
	if (!b_normales_vertices)
		calcular_normales_vertices();
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&ambiente_difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < caras.size(); ++i)
	{
		glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);

		glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);

		glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
	glDisable(GL_LIGHTING);
}

void _triangulos3D::calcular_normales_vertices()
{
	if (!b_normales_vertices)
	{
		if (!b_normales_caras)
			calcular_normales_caras(0);

		normales_vertices.resize(vertices.size());

		// Seteamos a 0
		for (auto normal_vertice : normales_vertices)
			normal_vertice = _vertex3f(0.0, 0.0, 0.0);

		// Se suman las normales de las caras que rodean a cada vertice (en cada cara se sacan los vertices que lo forman y se incrementa para luego dividirlo por el numero de veces que ha aparecido y asi normalizarlo)
		for (int i = 0; i < caras.size(); i++)
		{
			normales_vertices[caras[i]._0] += normales_caras[i];
			normales_vertices[caras[i]._1] += normales_caras[i];
			normales_vertices[caras[i]._2] += normales_caras[i];
		}

		// Funcion normalizar, se puede sustituir calculando antes las veces que aparece el vertice y dividir cada normal aqui por ese numero
		for (auto normal_vertice : normales_vertices)
			normal_vertice.normalize();

		b_normales_vertices = true;
	}
}

//*************************************************************************
// dibujar en modo seleccion
//*************************************************************************

void _triangulos3D::draw_seleccion(int r, int g, int b)
{
	int i;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3ub(r, g, b); // ub => unsigned byte
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	switch (modo)
	{
	case POINTS:
		draw_puntos(r1, g1, b1, grosor);
		break;
	case EDGES:
		draw_aristas(r1, g1, b1, grosor);
		break;
	case SOLID_CHESS:
		draw_solido_ajedrez(r1, g1, b1, r2, g2, b2);
		break;
	case SOLID:
		draw_solido(r1, g1, b1);
		break;
	case SELECT:
		draw_seleccion(r1, g1, b1);
	case SOLID_ILLUMINATED_FLAT:
		draw_iluminacion_plana();
		break;
	case SOLID_ILLUMINATED_GOURAUD:
		draw_iluminacion_suave();
		break;
	}
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
	// vertices
	vertices.resize(8);
	vertices[0].x = -tam;
	vertices[0].y = -tam;
	vertices[0].z = tam;
	vertices[1].x = tam;
	vertices[1].y = -tam;
	vertices[1].z = tam;
	vertices[2].x = tam;
	vertices[2].y = tam;
	vertices[2].z = tam;
	vertices[3].x = -tam;
	vertices[3].y = tam;
	vertices[3].z = tam;
	vertices[4].x = -tam;
	vertices[4].y = -tam;
	vertices[4].z = -tam;
	vertices[5].x = tam;
	vertices[5].y = -tam;
	vertices[5].z = -tam;
	vertices[6].x = tam;
	vertices[6].y = tam;
	vertices[6].z = -tam;
	vertices[7].x = -tam;
	vertices[7].y = tam;
	vertices[7].z = -tam;

	// triangulos
	caras.resize(12);
	caras[0]._0 = 0;
	caras[0]._1 = 1;
	caras[0]._2 = 3;
	caras[1]._0 = 3;
	caras[1]._1 = 1;
	caras[1]._2 = 2;
	caras[2]._0 = 1;
	caras[2]._1 = 5;
	caras[2]._2 = 2;
	caras[3]._0 = 5;
	caras[3]._1 = 6;
	caras[3]._2 = 2;
	caras[4]._0 = 5;
	caras[4]._1 = 4;
	caras[4]._2 = 6;
	caras[5]._0 = 4;
	caras[5]._1 = 7;
	caras[5]._2 = 6;
	caras[6]._0 = 0;
	caras[6]._1 = 7;
	caras[6]._2 = 4;
	caras[7]._0 = 0;
	caras[7]._1 = 3;
	caras[7]._2 = 7;
	caras[8]._0 = 3;
	caras[8]._1 = 2;
	caras[8]._2 = 7;
	caras[9]._0 = 2;
	caras[9]._1 = 6;
	caras[9]._2 = 7;
	caras[10]._0 = 0;
	caras[10]._1 = 1;
	caras[10]._2 = 4;
	caras[11]._0 = 1;
	caras[11]._1 = 5;
	caras[11]._2 = 4;
}

//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{

	// vertices
	vertices.resize(5);
	vertices[0].x = -tam;
	vertices[0].y = 0;
	vertices[0].z = tam;
	vertices[1].x = tam;
	vertices[1].y = 0;
	vertices[1].z = tam;
	vertices[2].x = tam;
	vertices[2].y = 0;
	vertices[2].z = -tam;
	vertices[3].x = -tam;
	vertices[3].y = 0;
	vertices[3].z = -tam;
	vertices[4].x = 0;
	vertices[4].y = al;
	vertices[4].z = 0;

	caras.resize(6);
	caras[0]._0 = 0;
	caras[0]._1 = 1;
	caras[0]._2 = 4;
	caras[1]._0 = 1;
	caras[1]._1 = 2;
	caras[1]._2 = 4;
	caras[2]._0 = 2;
	caras[2]._1 = 3;
	caras[2]._2 = 4;
	caras[3]._0 = 3;
	caras[3]._1 = 0;
	caras[3]._2 = 4;
	caras[4]._0 = 3;
	caras[4]._1 = 1;
	caras[4]._2 = 0;
	caras[5]._0 = 3;
	caras[5]._1 = 2;
	caras[5]._2 = 1;
}

//************************************************************************
// Clase triangulo
//************************************************************************
_triangulo::_triangulo(float alt, float anch, float prof)
{
	vertices.resize(6);
	vertices[0].x = 0;
	vertices[0].y = 0;
	vertices[0].z = anch;
	vertices[1].x = 0;
	vertices[1].y = -alt;
	vertices[1].z = anch;
	vertices[2].x = prof;
	vertices[2].y = 0;
	vertices[2].z = anch;
	vertices[3].x = 0;
	vertices[3].y = 0;
	vertices[3].z = 0;
	vertices[4].x = 0;
	vertices[4].y = -alt;
	vertices[4].z = 0;
	vertices[5].x = prof;
	vertices[5].y = 0;
	vertices[5].z = 0;

	caras.resize(8);
	caras[0]._0 = 0;
	caras[0]._1 = 1;
	caras[0]._2 = 2;
	caras[1]._0 = 3;
	caras[1]._1 = 4;
	caras[1]._2 = 5;
	caras[2]._0 = 2;
	caras[2]._1 = 5;
	caras[2]._2 = 4;
	caras[3]._0 = 2;
	caras[3]._1 = 4;
	caras[3]._2 = 1;
	caras[4]._0 = 0;
	caras[4]._1 = 3;
	caras[4]._2 = 5;
	caras[5]._0 = 0;
	caras[5]._1 = 5;
	caras[5]._2 = 2;
	caras[6]._0 = 0;
	caras[6]._1 = 3;
	caras[6]._2 = 4;
	caras[7]._0 = 0;
	caras[7]._1 = 4;
	caras[7]._2 = 1;
}
//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply()
{
	// leer lista de coordenadas de vértices y lista de indices de vértices
}

int _objeto_ply::parametros(char *archivo)
{
	int n_ver, n_car;

	vector<float> ver_ply;
	vector<int> car_ply;

	_file_ply::read(archivo, ver_ply, car_ply);

	n_ver = ver_ply.size() / 3;
	n_car = car_ply.size() / 3;

	printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

	vertices.resize(n_ver);
	caras.resize(n_car);

	_vertex3f ver_aux;
	_vertex3i car_aux;

	for (int i = 0; i < n_ver; i++)
	{
		ver_aux.x = ver_ply[i * 3];
		ver_aux.y = ver_ply[i * 3 + 1];
		ver_aux.z = ver_ply[i * 3 + 2];
		vertices[i] = ver_aux;
	}

	for (int i = 0; i < n_car; i++)
	{
		car_aux.x = car_ply[i * 3];
		car_aux.y = car_ply[i * 3 + 1];
		car_aux.z = car_ply[i * 3 + 2];
		caras[i] = car_aux;
	}

	return (0);
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{
}

/**
 * @brief Rotacion proporcionada por el profesorado
 * 
 * @param perfil 
 * @param num 
 * @param tapa 
 */
void _rotacion::parametros(vector<_vertex3f> perfil, int num, int tapa)
{
	int i, j;
	_vertex3f vertice_aux;
	_vertex3i cara_aux;
	int num_aux;

	// tratamiento de los vértice

	num_aux = perfil.size();
	vertices.resize(num_aux * num);
	for (j = 0; j < num; j++)
	{
		for (i = 0; i < num_aux; i++)
		{
			vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) +
							perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
			vertice_aux.z = -perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
							perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
			vertice_aux.y = perfil[i].y;
			vertices[i + j * num_aux] = vertice_aux;
		}
	}

	// tratamiento de las caras

	for (j = 0; j < num; j++)
	{
		for (i = 0; i < num_aux - 1; i++)
		{
			cara_aux._0 = i + ((j + 1) % num) * num_aux;
			cara_aux._1 = i + 1 + ((j + 1) % num) * num_aux;
			cara_aux._2 = i + 1 + j * num_aux;
			caras.push_back(cara_aux);

			cara_aux._0 = i + 1 + j * num_aux;
			cara_aux._1 = i + j * num_aux;
			cara_aux._2 = i + ((j + 1) % num) * num_aux;
			caras.push_back(cara_aux);
		}
	}

	// tapa inferior
	if (fabs(perfil[0].x) > 0.0 && tapa == 1)
	{
	}

	// tapa superior
	if (fabs(perfil[num_aux - 1].x) > 0.0 && tapa == 1)
	{
	}
}

/**
 * @brief Función de rotación para volúmenes cerrados
 * 
 * @author Juan Carlos Ruiz Fernández
 * 
 * @param perfil Perfil de puntos que se van a rotar
 * @param num Numero de veces que se va a rotar (resolución)
 * @param eje Eje de rotación, por defecto el vertical 'Y'
 * 
 */
void _rotacion::parametros(vector<_vertex3f> perfil, int num, char eje = 'y')
{

	int i, j;
	_vertex3f vertice_aux;
	_vertex3i cara_aux;
	_vertex3f vertice_inferior;
	_vertex3f vertice_superior;
	int num_aux, num_vertices;
	int modo;

	// tratamiento de los vértices
	if ((eje == 'y' && perfil[0].x != 0 && perfil[perfil.size() - 1].x != 0) ||
		(eje == 'x' && perfil[0].z != 0 && perfil[perfil.size() - 1].z != 0) ||
		(eje == 'z' && perfil[0].y != 0 && perfil[perfil.size() - 1].y != 0))
		modo = 0; // vertices en eje no creados

	else if ((eje == 'y' && perfil[0].x == 0 && perfil[perfil.size() - 1].x != 0) ||
			 (eje == 'x' && perfil[0].z == 0 && perfil[perfil.size() - 1].z != 0) ||
			 (eje == 'z' && perfil[0].y == 0 && perfil[perfil.size() - 1].y != 0))
		modo = 1; // solo primer vertice en eje(inferior)

	else if ((eje == 'y' && perfil[0].x != 0 && perfil[perfil.size() - 1].x == 0) ||
			 (eje == 'x' && perfil[0].z != 0 && perfil[perfil.size() - 1].z == 0) ||
			 (eje == 'z' && perfil[0].y != 0 && perfil[perfil.size() - 1].y == 0))
		modo = 2; // solo ultimo vertice en eje(superior)

	else
		modo = 3; // ambos vertices en eje

	//Para cada caso se crearán, o no, los vertices situados en el vertice para "cerrar" el volumen 
	switch (modo)
	{
	case 0:
		num_aux = perfil.size();	  // Vertices del perfil sin contar extremos
		num_vertices = num_aux * num; // Vertices totales sin extremos
		vertices.resize(num_vertices);
		// Calculamos vertice inferior
		vertice_inferior.x = (eje == 'x') ? perfil[0].x : 0;
		vertice_inferior.y = (eje == 'y') ? perfil[0].y : 0;
		vertice_inferior.z = (eje == 'z') ? perfil[0].z : 0;
		// Calculamos vertice superior
		vertice_superior.x = (eje == 'x') ? perfil[num_aux - 1].x : 0;
		vertice_superior.y = (eje == 'y') ? perfil[num_aux - 1].y : 0;
		vertice_superior.z = (eje == 'z') ? perfil[num_aux - 1].z : 0;
		break;

	case 1:
		num_aux = perfil.size() - 1;
		num_vertices = num_aux * num;
		vertices.resize(num_vertices);
		// Extraemos vertice inferior
		vertice_inferior = perfil[0];
		// Calculamos vertice superior
		vertice_superior.x = (eje == 'x') ? perfil[num_aux].x : 0;
		vertice_superior.y = (eje == 'y') ? perfil[num_aux].y : 0;
		vertice_superior.z = (eje == 'z') ? perfil[num_aux].z : 0;
		// desplazar para eliminar del vector el vertice extremo basura
		for (int i = 1; i < perfil.size(); i++)
			perfil[i - 1] = perfil[i];
		perfil.pop_back();
		break;

	case 2:
		num_aux = perfil.size() - 1;
		num_vertices = num_aux * num;
		vertices.resize(num_vertices);
		// Extraemos vertice inferior
		vertice_superior = perfil[num_aux];
		// Calculamos vertice superior
		vertice_inferior.x = (eje == 'x') ? perfil[0].x : 0;
		vertice_inferior.y = (eje == 'y') ? perfil[0].y : 0;
		vertice_inferior.z = (eje == 'z') ? perfil[0].z : 0;
		// eliminar del vector el vertice extremo basura
		perfil.pop_back();

		break;

	case 3:							  // ambos vertices incluidos
		num_aux = perfil.size() - 2;  // Vertices del perfil sin contar extremos
		num_vertices = num_aux * num; // Vertices totales sin extremos
		vertices.resize(num_vertices);
		vertice_inferior = perfil[0];
		vertice_superior = perfil[perfil.size() - 1];
		// desplazar para eliminar del vector los dos vertices extremos basura
		for (int i = 1; i < (perfil.size() - 1); i++)
			perfil[i - 1] = perfil[i];
		perfil.pop_back();
		perfil.pop_back();
		break;
	}

	// Rotación de vertices
	for (j = 0; j < num; j++)
	{
		for (i = 0; i < num_aux; i++)
		{
			if (eje == 'y')
			{
				vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) +
								perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
				vertice_aux.z = -perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
								perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
				vertice_aux.y = perfil[i].y;
			}
			else if (eje == 'x')
			{
				vertice_aux.y = perfil[i].y * cos(2.0 * M_PI * j / (1.0 * num)) -
								perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
				vertice_aux.z = perfil[i].y * sin(2.0 * M_PI * j / (1.0 * num)) +
								perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
				vertice_aux.x = perfil[i].x;
			}
			else
			{
				vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) -
								perfil[i].y * sin(2.0 * M_PI * j / (1.0 * num));
				vertice_aux.y = perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
								perfil[i].y * cos(2.0 * M_PI * j / (1.0 * num));
				vertice_aux.z = perfil[i].z;
			}
			vertices[i + j * num_aux] = vertice_aux;
		}
	}

	// Inserccion de los vertices extremos de los ejes
	vertices.push_back(vertice_inferior);
	vertices.push_back(vertice_superior);

	// Genereacion de caras
	for (int i = 0; i < num_vertices - 1; i++)
	{
		// Salto a siguiente columna si he llegado arriba
		if (i % num_aux == (num_aux - 1))
			i++;

		// tringulo inferior
		cara_aux._0 = i;
		cara_aux._1 = (i + num_aux) % num_vertices;
		cara_aux._2 = i + 1;
		caras.push_back(cara_aux);

		// triangulo superior
		cara_aux._0 = i + 1;
		cara_aux._1 = (i + num_aux) % num_vertices;
		cara_aux._2 = (i + num_aux + 1) % num_vertices;
		caras.push_back(cara_aux);
	}

	// tapa inferior
	for (int i = 0; i < num; i++)
	{
		cara_aux._0 = i * num_aux;
		cara_aux._1 = (i * num_aux + num_aux) % num_vertices;
		cara_aux._2 = num_vertices; // tapa inferior
		caras.push_back(cara_aux);
	}

	// tapa superior
	for (int i = 0; i < num; i++)
	{
		cara_aux._0 = num_aux - 1 + i * num_aux;
		cara_aux._1 = (num_aux - 1 + (i + 1) * num_aux) % num_vertices;
		cara_aux._2 = num_vertices + 1; // tapa superior
		caras.push_back(cara_aux);
	}

}

/**
 * @brief Construct a new cono:: cono object
 *
 * @param diametro
 * @param altura
 * @param eje
 */
_cono::_cono(float diametro, float altura, char eje)
{
	vector<_vertex3f> perfil;
	_vertex3f punto;
	_rotacion cono;

	// formar perfil
	punto.x = (eje == 'y') ? diametro : 0;
	punto.y = (eje == 'z') ? diametro : 0;
	punto.z = (eje == 'x') ? diametro : 0;
	perfil.push_back(punto);

	punto.x = (eje == 'x') ? altura : 0;
	punto.y = (eje == 'y') ? altura : 0;
	punto.z = (eje == 'z') ? altura : 0;
	perfil.push_back(punto);
	printf("\n%d\n", perfil.size());
	// formar revolucion
	cono.parametros(perfil, 15, eje);
	vertices = cono.vertices;
	caras = cono.caras;
}

_arbol::_arbol(float diametro, float altura)
{
	this->altura = altura;
	vector<_vertex3f> perfil;
	_vertex3f punto;

	// formar tronco
	punto.x = diametro / 4;
	punto.y = 0;
	punto.z = 0;
	perfil.push_back(punto);
	punto.x = diametro / 4;
	punto.y = altura / 2;
	punto.z = 0;
	perfil.push_back(punto);
	tronco.parametros(perfil, 10, 'y');

	// //Crear copas
	copa1 = new _cono(diametro, altura / 2, 'y');
	copa2 = new _cono(diametro / 2, altura / 2, 'y');

	copa1->ambiente_difusa = _vertex4f(0.0, 0.7, 0, 1);
	copa2->ambiente_difusa = _vertex4f(0.0, 0.8, 0, 1);
	tronco.ambiente_difusa = _vertex4f(0.5, 0.2, 0, 1);
}

void _arbol::draw(_modo modo, float grosor)
{
	tronco.draw(modo, rojo, verde3, 0, rojo + 0.1, verde3, 0, grosor);
	glTranslatef(0, altura / 2, 0);
	copa1->draw(modo, 0, verde1, 0, 0, verde2, 0, grosor);
	glTranslatef(0, altura / 4, 0);
	copa2->draw(modo, 0, verde1, 0, 0, verde2, 0, grosor);
}


_entorno::_entorno()
{
	arbol = new _arbol(1, 2);
	// Generacion de posicion de arboles
	posiciones = new int *[tamx];
	for (int i = 0; i < tamx; i++)
	{
		posiciones[i] = new int[tamz];
		for (int j = 0; j < tamz; j++)
			posiciones[i][j] = random(5, tamz + 5);
	}

	cielo_esfera = _esfera(100, 128, 1);

	cielo_esfera.esfera.ambiente_difusa = _vertex4f(0.4, 0.15, 0.1, 1);
	cielo_esfera.esfera.especular = _vertex4f(1, 1, 1, 1);
	cielo_esfera.esfera.brillo = 200;
	suelo.ambiente_difusa = _vertex4f(0.8, 0.5, 0.5, 1);
}

void _entorno::draw(_modo modo, float grosor)
{
	glPushMatrix();
	glScalef(100, 0.1, 100);
	glTranslatef(0, -1, 0);
	suelo.draw(modo, 0.4, 0.2, 0, 0.4, 0.1, 0, grosor);
	glPopMatrix();

	cielo_esfera.draw(modo, 0.7, 0.7, 0.7, 0.8, 0.8, 0.8, grosor);

	// // Suelo de 1000 cubos para ver reflectadas las luces
	//  glPushMatrix();
	//  glScalef(0.1, 0.1, 0.1);
	//  glTranslatef(0, -0.5, 0);
	//  for( int i = 0; i < 100; i++){
	//  	for( int j = 0; j < 100; j++){
	//  		glPushMatrix();
	//  		glTranslatef(-50+i, 0, -50+j);
	//  		suelo.draw(modo, 0.4, 0.2, 0, 0.4, 0.1, 0, grosor);
	//  		glPopMatrix();
	//  	}
	//  }
	//  glPopMatrix();

	for (int i = 0; i < tamx; i++)
		for (int j = 0; j < tamz; j++)
		{
			glPushMatrix();
			glTranslatef(i - tamx / 2, 0, -1 * posiciones[i][j]);
			arbol->draw(modo, grosor);
			glPopMatrix();
		}
}

//************************************************************************
// objeto articulado: tanque
//************************************************************************

_chasis::_chasis()
{
	// perfil para un cilindro
	vector<_vertex3f> perfil;
	_vertex3f aux;
	aux.x = 0.107;
	aux.y = -0.5;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 0.107;
	aux.y = 0.5;
	aux.z = 0.0;
	perfil.push_back(aux);
	rodamiento.parametros(perfil, 12, 1);
	altura = 0.22;

	rodamiento.ambiente_difusa = _vertex4f(1, 0.5, 0, 1);
	base.ambiente_difusa = _vertex4f(1, 0.5, 0, 1);
};

void _chasis::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	glPushMatrix();
	glScalef(1.0, 0.22, 0.95);
	base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, 0.0, 0.0);
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.0, 0.0);
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0.0, 0.0);
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.0, 0.0);
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();
}

//************************************************************************

_torreta::_torreta()
{
	altura = 0.18;
	anchura = 0.65;
};

void _torreta::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	glPushMatrix();
	glScalef(0.65, 0.18, 0.6);
	base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.325, 0, 0);
	glRotatef(90.0, 0, 0, 1);
	glScalef(0.18, 0.16, 0.6);
	parte_trasera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();
}

//************************************************************************

_tubo::_tubo()
{
	// perfil para un cilindro
	vector<_vertex3f> perfil;
	_vertex3f aux;
	aux.x = 0.04;
	aux.y = -0.4;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 0.04;
	aux.y = 0.4;
	aux.z = 0.0;
	perfil.push_back(aux);
	tubo_abierto.parametros(perfil, 12, 0);
};

void _tubo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

	glPushMatrix();
	glTranslatef(0.4, 0, 0);
	glRotatef(90.0, 0, 0, 1);
	tubo_abierto.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();
}

//************************************************************************

_tanque::_tanque()
{
	int c = 100;
	giro_tubo = 2.0;
	giro_torreta = 0.0;
	giro_tubo_min = -9;
	giro_tubo_max = 20;

	piezas = 3;
	color_pick[0] = 1.0;
	color_pick[1] = 0.0;
	color_pick[2] = 0.0;
	for (int i = 0; i < piezas; i++)
	{
		activo[i] = 0;
		cambio[i] = 0;
		color_selec[0][i] = color_selec[1][i] = color_selec[2][i] = c;
		c = c + 20; // Color diferente a cada pieza
	}
};

void _tanque::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	float r_p, g_p, b_p;

	r_p = color_pick[0];
	g_p = color_pick[1];
	b_p = color_pick[2];

	glPushMatrix();
	if (activo[0] == 1)
		chasis.draw(modo, r_p, g_p, b_p, r_p, g_p, b_p, grosor);
	else
		chasis.draw(modo, r1, g1, b1, r2, g2, b2, grosor);

	glRotatef(giro_torreta, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0.0, (chasis.altura + torreta.altura) / 2.0, 0.0);
	if (activo[1] == 1)
		torreta.draw(modo, r_p, g_p, b_p, r_p, g_p, b_p, grosor);
	else
		torreta.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(torreta.anchura / 2.0, (chasis.altura + torreta.altura) / 2.0, 0.0);
	glRotatef(giro_tubo, 0, 0, 1);
	if (activo[2] == 1)
		tubo.draw(modo, r_p, g_p, b_p, r_p, g_p, b_p, grosor);
	else
		tubo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();
	glPopMatrix();
}

void _tanque::seleccion()
{
	int c;
	c = color_selec[0][0];
	glPushMatrix();
	chasis.draw(SELECT, c, c, c, c, c, c, 1);

	c = color_selec[0][1];
	glRotatef(giro_torreta, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0.0, (chasis.altura + torreta.altura) / 2.0, 0.0);
	torreta.draw(SELECT, c, c, c, c, c, c, 1);
	glPopMatrix();

	c = color_selec[0][2];
	glPushMatrix();
	glTranslatef(torreta.anchura / 2.0, (chasis.altura + torreta.altura) / 2.0, 0.0);
	glRotatef(giro_tubo, 0, 0, 1);
	tubo.draw(SELECT, c, c, c, c, c, c, 1);
	glPopMatrix();
	glPopMatrix();
}

//************************************************************************
// objeto articulado: retro excavadora
//************************************************************************
_Retro_Excavadora_chasis::_Retro_Excavadora_chasis()
{
	// perfil para un cilindro
	vector<_vertex3f> perfil;
	_vertex3f aux;
	aux.x = 0.5;
	aux.y = -1;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 0.5;
	aux.y = 1;
	aux.z = 0.0;
	perfil.push_back(aux);
	rodamiento.parametros(perfil, 12, 'y');
	rodamiento1.parametros(perfil, 12, 'y');

	rodamiento.ambiente_difusa = _vertex4f(0.2, 0.2, 0.2, 1);
	rodamiento1.ambiente_difusa = _vertex4f(0.8, 0.8, 0.8, 1);
};

void _Retro_Excavadora_chasis::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor, float velocidad)
{

	if(r1 == r2){
	_vertex4f v = _vertex4f(0.8, 0.1, 0.1, 1.0);	
	_vertex4f r = rodamiento.ambiente_difusa;	
	_vertex4f r1 = rodamiento1.ambiente_difusa;	
	rodamiento.ambiente_difusa = v;
	rodamiento1.ambiente_difusa =v;
	}

	float angulo = angulo_avance * velocidad * -1;

	glPushMatrix();
	glScalef(1, 1, 2);
	glTranslatef(-2, 0.5, 0.0);
	glRotatef(angulo, 0, 0, 1);
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glScalef(0.7, 1.05, 0.7);
	rodamiento1.draw(modo, r1 + 0.5, g1 + 0.5, b1 + 0.5, r2 + 0.5, g2 + 0.5, b2 + 0.5, grosor);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 2);
	glTranslatef(2, 0.5, 0.0);
	glRotatef(angulo, 0, 0, 1);
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glScalef(0.7, 1.05, 0.7);
	rodamiento1.draw(modo, r1 + 0.5, g1 + 0.5, b1 + 0.5, r2 + 0.5, g2 + 0.5, b2 + 0.5, grosor);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 2);
	glTranslatef(1, 0.5, 0.0);
	glRotatef(angulo, 0, 0, 1);
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glScalef(0.7, 1.05, 0.7);
	rodamiento1.draw(modo, r1 + 0.5, g1 + 0.5, b1 + 0.5, r2 + 0.5, g2 + 0.5, b2 + 0.5, grosor);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 2);
	glTranslatef(-1, 0.5, 0.0);
	glRotatef(angulo, 0, 0, 1);
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glScalef(0.7, 1.05, 0.7);
	rodamiento1.draw(modo, r1 + 0.5, g1 + 0.5, b1 + 0.5, r2 + 0.5, g2 + 0.5, b2 + 0.5, grosor);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 2);
	glTranslatef(0.0, 0.5, 0.0);
	glRotatef(angulo, 0, 0, 1);
	glRotatef(90.0, 1, 0, 0);
	rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glScalef(0.7, 1.05, 0.7);
	rodamiento1.draw(modo, r1 + 0.5, g1 + 0.5, b1 + 0.5, r2 + 0.5, g2 + 0.5, b2 + 0.5, grosor);
	glPopMatrix();

	rodamiento.ambiente_difusa = _vertex4f(0.2, 0.2, 0.2, 1);
	rodamiento1.ambiente_difusa = _vertex4f(0.8, 0.8, 0.8, 1);
}

_Retro_Excavadora_cuerpo::_Retro_Excavadora_cuerpo()
{
	vector<_vertex3f> perfil;
	_vertex3f aux;
	aux.x = 0.5;
	aux.y = -1;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 0.5;
	aux.y = 1;
	aux.z = 0.0;
	perfil.push_back(aux);
	rodamiento.parametros(perfil, 12, 'y');

	rodamiento.ambiente_difusa = _vertex4f(1, 0.5, 0, 1);
	cuerpo.ambiente_difusa = _vertex4f(1, 0.5, 0, 1);
	cabina.ambiente_difusa(0.2, 0.2, 0.2, 1);
	cabina1.ambiente_difusa(0.2, 0.2, 0.8, 1);
}

void _Retro_Excavadora_cuerpo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	if(r1 == r2){
	_vertex4f v = _vertex4f(0.8, 0.1, 0.1, 1.0);	
	_vertex4f v1 = _vertex4f(0.9, 0.2, 0.2, 1.0);	
	_vertex4f r = rodamiento.ambiente_difusa;	
	_vertex4f b = cuerpo.ambiente_difusa;
	_vertex4f c = cabina.ambiente_difusa;
	_vertex4f c1 = cabina1.ambiente_difusa;
	rodamiento.ambiente_difusa = v;
	cuerpo.ambiente_difusa = v;
	cabina.ambiente_difusa = v;
	cabina1.ambiente_difusa= v1;
	}

	glPushMatrix();
	// glScalef(1,2.0,1.0);
	glScalef(1, 0.99, 1);
	glTranslatef(-2.5, 2, 1.5);
	if( modo == SELECT )
		rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	else 
		rodamiento.draw(modo, r1-0.2, g1-0.2, b1-0.2, r2-0.2, g2-0.2, b2-0.2, grosor);
	glTranslatef(0, 0, -3);
	if( modo == SELECT )
		rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	else 
		rodamiento.draw(modo, r1-0.2, g1-0.2, b1-0.2, r2-0.2, g2-0.2, b2-0.2, grosor);
	glTranslatef(5, 0, 0);
	if( modo == SELECT )
		rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	else 
		rodamiento.draw(modo, r1-0.2, g1-0.2, b1-0.2, r2-0.2, g2-0.2, b2-0.2, grosor);
	glTranslatef(0, 0, 3);
	if( modo == SELECT )
		rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	else 
		rodamiento.draw(modo, r1-0.2, g1-0.2, b1-0.2, r2-0.2, g2-0.2, b2-0.2, grosor);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1.99, 3);
	glTranslatef(-2.5, 1, 0);
	cuerpo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glTranslatef(5, 0, 0);
	cuerpo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	// glScalef(0.1,0.1,0.1);
	glScalef(5, 2, 4);
	glTranslatef(0.0, 1, 0.0);
	cuerpo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	// glScalef(0.1,0.1,0.1);
	glTranslatef(1.5, 3.5, 1.5);
	if( modo == SELECT )
		cabina.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	else if( r1==r2 )
		cabina.draw(modo, r1-0.2, g1-0.2, b1-0.2, r2-0.2, g2-0.2, b2-0.2, grosor);
	else
		cabina.draw(modo, 0, 0, 0, 0.3, 0.3, 0.3, grosor);
	glPushMatrix();
	glScalef(1.1, 0.7, 0.7);
	if( modo == SELECT )
		cabina1.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	else if( r1==r2 )
		cabina1.draw(modo, r1+0.5, g1+0.5, b1+0.5, r2+0.5, g2+0.5, b2+0.5, grosor);
	else{
		cabina1.draw(modo, 0, 0.5, 0.9, 0, 0.5, 0.6, grosor);
	}
	glPopMatrix();
	glPushMatrix();
	glScalef(0.7, 0.7, 1.1);
	if( modo == SELECT )
		cabina1.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	else if( r1==r2 )
		cabina1.draw(modo, r1+0.5, g1+0.5, b1+0.5, r2+0.5, g2+0.5, b2+0.5, grosor);
	else{
		cabina1.draw(modo, 0, 0.5, 0.9, 0, 0.5, 0.6, grosor);
	}
	glPopMatrix();
	glPopMatrix();

	rodamiento.ambiente_difusa = _vertex4f(1, 0.5, 0, 1);
	cuerpo.ambiente_difusa = _vertex4f(1, 0.5, 0, 1);
	cabina.ambiente_difusa(0.2, 0.2, 0.2, 1);
	cabina1.ambiente_difusa(0.2, 0.2, 0.8, 1);
}

_Retro_Excavadora_brazo::_Retro_Excavadora_brazo()
{
	giro_brazo1 = 45;
	giro_brazo2 = -90;
	giro_mano = 0;
	separacion = 1;

	vector<_vertex3f> perfil;
	_vertex3f aux;
	aux.x = 1.2;
	aux.y = 0;
	aux.z = 0;
	perfil.push_back(aux);
	aux.x = 1.2;
	aux.y = 0.1;
	aux.z = 0;
	perfil.push_back(aux);
	rodamiento.parametros(perfil, 12, 'y');

	rodamiento.ambiente_difusa = _vertex4f(0.1, 0.1, 0.1, 1.0);

	perfil.clear();
	aux.x = 0.5;
	aux.y = -1;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 0.5;
	aux.y = 1;
	aux.z = 0.0;
	perfil.push_back(aux);

	rodamiento1.parametros(perfil, 12, 'y');
	rodamiento1.ambiente_difusa = _vertex4f(0.1, 0.1, 0.1, 1.0);
	brazos.ambiente_difusa = _vertex4f(1, 0.5, 0, 1);
}

void _Retro_Excavadora_brazo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	if(r1 == r2){
	_vertex4f v = _vertex4f(0.8, 0.1, 0.1, 1.0);	
	_vertex4f r = rodamiento1.ambiente_difusa;	
	_vertex4f b = brazos.ambiente_difusa;
	rodamiento1.ambiente_difusa =v;
	rodamiento.ambiente_difusa =v;
	brazos.ambiente_difusa = v;
	}
	
	glPushMatrix();
	glTranslatef(2.5, 3.5, -1);		 // Mover punto de rotacion
	glRotatef(giro_brazo1, 0, 0, 1); // generar rotacion
	glPushMatrix();
	glTranslatef(2, 0, 0); // desplazar figura respecto punto de rotacion
	glScalef(4, 1, 2);
	brazos.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 0.99);
	glRotatef(90, 1, 0, 0);
	if(r1 == r2)
		rodamiento1.draw(modo, r1-0.2, g1-0.2, b1-0.2, r2-0.2, g2-0.2, b2-0.2, grosor);
	else{
		rodamiento1.draw(modo, negro_1, negro_1, negro_1, negro_2, negro_2, negro_2, grosor);
	}
	//rodamiento1.draw(modo, negro_1, negro_1, negro_1, negro_2, negro_2, negro_2, grosor);
	
	glPopMatrix();

	glTranslatef(4, 0, 0);
	glRotatef(giro_brazo2, 0, 0, 1);
	glPushMatrix();
	glTranslatef(0.5, 0, 0);
	glScalef(1, 1, 0.99);
	glRotatef(90, 1, 0, 0);
	if(r1 == r2)
		rodamiento1.draw(modo, r1-0.2, g1-0.2, b1-0.2, r2-0.2, g2-0.2, b2-0.2, grosor);
	else{
		rodamiento1.draw(modo, negro_1, negro_1, negro_1, negro_2, negro_2, negro_2, grosor);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.5, 0, 0);
	glScalef(4, 1, 2);
	brazos.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix(); // mano
	glTranslatef(4.6, 0, 0);
	glRotatef(giro_mano, 1, 0, 0);
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	if(r1 == r2)
		rodamiento.draw(modo, r1-0.2, g1-0.2, b1-0.2, r2-0.2, g2-0.2, b2-0.2, grosor);
	else{
		rodamiento.draw(modo, negro_1, negro_1, negro_1, negro_2, negro_2, negro_2, grosor);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0, -separacion);
	glScalef(1, 1, 0.2);
	brazos.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.5, 0, separacion);
	glScalef(1, 1, 0.2);
	brazos.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	// glPushMatrix();
	// glScalef(4,1,2);
	// glPopMatrix();
	rodamiento1.ambiente_difusa = _vertex4f(0.1, 0.1, 0.1, 1.0);
	rodamiento.ambiente_difusa = _vertex4f(0.1, 0.1, 0.1, 1.0);
	brazos.ambiente_difusa = _vertex4f(1, 0.5, 0, 1);
}

//***********************************************************

void _Retro_Excavadora::seleccion(){
	int c;
	glPushMatrix();
		glTranslatef(posx, 0, posz);
		c = color_selec[0][0];
		chasis.draw(SELECT, c, c, c, c, c, c, 1, posx);
		glPushMatrix();
			glRotatef(giro_base, 0, 1, 0);
			c = color_selec[0][1];
			cuerpo.draw(SELECT, c, c, c, c, c, c, 1);
			glPushMatrix();
				c = color_selec[0][2];
				brazo.draw(SELECT, c, c, c, c, c, c, 1);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

_Retro_Excavadora::_Retro_Excavadora()
{
	float c = 50;

	giro_base = 0;
	posx = 0;
	posz = 0;
	entorno = new _entorno();

	color_pick[0] = 1.0;
	color_pick[1] = 0.0;
	color_pick[2] = 0.0;

	for (int i = 0; i < piezas; i++, c+=50)
	{
		activo[i] = 0;
		cambio[i] = 0;
		color_selec[0][i] = color_selec[1][i] = color_selec[2][i] = c;
	}
};

void _Retro_Excavadora::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor, float velocidad, bool paisaje)
{
	float r_p, g_p, b_p;

	r_p = color_pick[0];
	g_p = color_pick[1];
	b_p = color_pick[2];

	if(paisaje){
		glPushMatrix();
		glScalef(15, 15, 15);
		entorno->draw(modo, grosor);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(posx, 0, posz);
	if(activo[0])
		chasis.draw(modo, r_p, g_p, b_p, r_p, g_p, b_p, grosor, posx);
	else
		chasis.draw(modo, negro_1, negro_1, negro_1, negro_2, negro_2, negro_2, grosor, posx);
	glPushMatrix();
	glRotatef(giro_base, 0, 1, 0);
	if(activo[1])
		cuerpo.draw(modo, r_p, g_p, b_p, r_p, g_p, b_p, grosor);
	else
		cuerpo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPushMatrix();
	if(activo[2])
		brazo.draw(modo, r_p, g_p, b_p, r_p, g_p, b_p, grosor);
	else
		brazo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
};

//************************************************************************

_esfera::_esfera(float diametro, int precision, int inv)
{
	vector<_vertex3f> perfil;
	_vertex3f punto;
	//_rotacion esfera;
	punto.x = 0;
	punto.y = -diametro / 2;
	punto.z = 0;
	perfil.push_back(punto);

	esfera.parametros(perfil, precision, 'z');
	perfil = esfera.vertices;

	for (int i = 0; i < precision / 2; i++)
		perfil.pop_back(); // me quedo con media esfera
	esfera.caras.clear();

	vector<_vertex3f>::iterator it = perfil.end();
	it--;
	it->x = 0;

	// printf("MODO DE la esfera");
	esfera.parametros(perfil, precision, 'y');
	// vertices = esfera.vertices;
	// caras = esfera.caras;

	// NORMALES VERTICES
	if (inv)
		calcular_normales_caras(inv);
	calcular_normales_vertices();

	// normales_vertices.resize(vertices.size());
	// for (int i = 0; i < vertices.size(); i++)
	// 	normales_vertices[i] = vertices[i].normalize();
	// b_normales_vertices = true;
}
void _esfera::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	// esfera.ambiente_difusa = {0.7, 0.7, 0.7, 1.0};
	// esfera.especular       = {0.7, 0.7, 0.7, 1.0};
	// esfera.brillo          = 10;
	esfera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
}

// Esfera pequeña
//  esfera.ambiente_difusa = {0.7, 0.7, 0.7, 1.0};
//  esfera.especular       = {0.7, 0.7, 0.7, 1.0};
//  esfera.brillo          = 10;

// //Esfera grande
// esfera.ambiente_difusa = {0.7, 0.1, 0.1, 1.0},
// esfera.especular       = {0.7, 0.1, 0.1, 1.0},
// esfera.brillo          = 90;
// glScalef(300, 300, 300);
// esfera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
