/*
*
*Autores : Miguel Guimar�es, Miguel Rodrigues, Xavier Rodrigues, Emanuel Fernandes
*Data : 2016 - 03 - 01
*
*/

#define _USE_MATH_DEFINES
#include "Forma.h"

#include <glut.h>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <iostream>
#include <sstream>

using namespace std;

/*--------------------------  Class  ----------------------------*/
/*-------------------------- Ponto3D ----------------------------*/

Ponto3D::Ponto3D()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

/* Imprime Ponto3D */
void Ponto3D::print() const
{
	cout << "X: " << x << "Y: " << y << "Z: " << z << "\n";
}

/* Converte Ponto3D para string*/
string Ponto3D::toString()
{
	stringstream ss;
	ss << x << " " << y << " " << z;
	return ss.str();
}

/*-------------------------- Forma ----------------------------*/

/* Fun��o para ler Poontos de um ficheiro */
void Forma::readfromFile(string filename)
{
	std::fstream file(filename, std::ios_base::in);

	file >> nome;
	float x, y, z;
	while (file >> x >> y >> z)
	{
		Ponto3D p;
		p.x = x;
		p.y = y;
		p.z = z;
		tris.push_back(p);
	}
	file.close();
}

/*-------------------------- Triangulo ----------------------------*/

Triangulo::Triangulo()
{
	lado = 0.0f;
}

void Triangulo::geraTriangulo(float l, string filename)
{
	lado = l;
	writetoFile(filename);
}

/* Escreve um Triangulo para ficheiro .3d */
void Triangulo::writetoFile(string filename)
{
	float h = sinf(M_PI / 3) * (lado / 2);
	ofstream file(filename);

	file << "TRIANGULO\n";

	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	file << h << " " << 0.0f << " " << 0.0f << "\n";
	file << 0.0f << " " << 0.0f << " " << (-lado / 2) << "\n";

	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	file << 0.0f << " " << 0.0f << " " << (lado / 2) << "\n";
	file << h << " " << 0.0f << " " << 0.0f << "\n";

	file.close();
}

/* Desenha todos os Triangulos no vetor tris */
void Triangulo::draw()
{
	int n = tris.size();
	for (int i = 0; i < n; i += 3) {
		glBegin(GL_TRIANGLES);
		glColor3f(1, 1, 1);

		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
		glVertex3f(tris[i + 1].x, tris[i + 1].y, tris[i + 1].z);
		glVertex3f(tris[i + 2].x, tris[i + 2].y, tris[i + 2].z);
		glEnd();
	}
	return;
}

/*------------------------- C�rculo ----------------------------*/

Circulo::Circulo()
{
	lados = 0;
	raio = 0.0f;
}

void Circulo::geraCirculo(float r, int l, string filename)
{
	raio = r; lados = l;
	writetoFile(filename);
}

/* Escreve um Circulo para ficheiro .3d */
void Circulo::writetoFile(string filename)
{
	float alpha = 2 * M_PI;
	float decAngulo = (2 * M_PI) / lados;
	ofstream file(filename);

	file << "CIRCULO\n";
	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	for (int i = 0; i <= lados; i++) {
		file << raio*sinf(alpha) << " " << 0 << " " << raio*cosf(alpha) << "\n";
		alpha -= decAngulo;
	}
	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	for (int i = 0; i <= lados; i++) {
		file << raio*cosf(alpha) << " " << 0 << " " << raio*sinf(alpha) << "\n";
		alpha -= decAngulo;
	}

	file.close();
}

/* Desenha todos os Triangulos no vetor tris que devem formar um Circulo */
void Circulo::draw()
{
	int n = tris.size();
	int i;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 1);

	for (i = 0; i < n; i++) {
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
	}

	glEnd();
}

/*------------------------- Plano ----------------------------*/

Plano::Plano()
{
	c = 0.0f;
	l = 0.0f;
}
void Plano::geraPlano(float comprimento, float largura, string filename)
{
	c = comprimento;
	l = largura;
	writetoFile(filename);
}

/* Escreve um Plano para ficheiro .3d */
void Plano::writetoFile(string filename)
{
	ofstream file(filename);

	file << "PLANO\n";

	file << -l / 2.0f << " " << 0.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << 0.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << 0.0f << " " << -c / 2.0f << "\n";

	file << -l / 2.0f << " " << 0.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << 0.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << 0.0f << " " << -c / 2.0f << "\n";

	file << c / 2.0f << " " << 0.0f << " " << -l / 2.0f << "\n";
	file << -c / 2.0f << " " << 0.0f << " " << l / 2.0f << "\n";
	file << -c / 2.0f << " " << 0.0f << " " << -l / 2.0f << "\n";

	file << c / 2.0f << " " << 0.0f << " " << -l / 2.0f << "\n";
	file << c / 2.0f << " " << 0.0f << " " << l / 2.0f << "\n";
	file << -c / 2.0f << " " << 0.0f << " " << l / 2.0f << "\n";


	file.close();
}

/* Desenha todos os Triangulos no vetor tris que devem formar um Plano */
void Plano::draw()
{
	int n = tris.size();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);

	for (int i = 0; i < n; i++)
	{
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
	}

	glEnd();

}

/*------------------------- Caixa ----------------------------*/

Caixa::Caixa()
{
	c = 0.0f;
	l = 0.0f;
	a = 0.0f;
}

void Caixa::geraCaixa(float comprimento, float largura, float altura, string filename)
{
	c = comprimento;
	l = largura;
	a = altura;
	writetoFile(filename);
}

/* Escreve uma Caixa para ficheiro caixa.3d */
void Caixa::writetoFile(string filename)
{
	ofstream file(filename);

	file << "CAIXA\n";

	//base
	file << l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";

	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";

	//cima
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";

	file << l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";

	//esquerda
	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";

	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";

	//direita
	file << -l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";

	file << -l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";

	//frente
	file << -l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";

	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";

	//atras
	file << -l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";

	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";

	file.close();
}

/* Desenha todos os Triangulos no vetor tris que devem formar uma Caixa */
void Caixa::draw()
{
	int n = tris.size();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 1);

	for (int i = 0; i < n; i++) {
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
	}

	glEnd();
}

/*------------------------- Esfera ----------------------------*/

Esfera::Esfera()
{
	r = 0.0f;
	n = 0.0f;
	f = 0.0f;
}

void Esfera::geraEsfera(float raio, float niveis, float fatias, string filename)
{
	r = raio;
	n = niveis;
	f = fatias;
	writetoFile(filename);
}

/* Escreve uma Esfera para ficheiro .3d */
void Esfera::writetoFile(string filename)
{
	ofstream file(filename);
	file << "ESFERA\n";

	int i, j;
	double hstep = (2 * M_PI) / f;
	double vstep = (M_PI) / n;
	double altura = r*sin((M_PI / 2) - hstep);
	double alturaCima = r;

	for (i = 0; i < f; i++) {

		double actualX = r*sin(i*hstep);
		double actualZ = r*cos(i*vstep);
		double nextX = r*sin((i + 1)*vstep);
		double nextZ = r*cos((i + 1)*vstep);
		double actX, actZ, nexX, nexZ, cimaActX, cimaActZ, cimaNexX, cimaNexZ;

		for (j = 1; j < n + 2; j++) {


			double aux = cos(asin(altura / r));
			actX = actualX * aux;
			actZ = actualZ * aux;
			nexX = nextX * aux;
			nexZ = nextZ * aux;

			aux = cos(asin(alturaCima / r));
			cimaActX = actualX * aux;
			cimaActZ = actualZ * aux;
			cimaNexX = nextX * aux;
			cimaNexZ = nextZ * aux;

			file << actX << " " << altura << " " << actZ << "\n";
			file << cimaActX << " " << alturaCima << " " << cimaActZ << "\n";
			file << nexX << " " << altura << " " << nexZ << "\n";

			file << cimaActX << " " << alturaCima << " " << cimaActZ << "\n";
			file << cimaNexX << " " << alturaCima << " " << cimaNexZ << "\n";
			file << nexX << " " << altura << " " << nexZ << "\n";

			alturaCima = altura;
			altura = r* sin((M_PI / 2) - (hstep*j));

		}

		altura = r* sin((M_PI / 2) - hstep);
		alturaCima = r;

		actualX = nextX;
		actualZ = nextZ;

	}

	file.close();
}

/* Desenha todos os Triangulos no vetor tris que devem formar uma Esfera */
void Esfera::draw()
{
	int n = tris.size();
	for (int i = 0; i < n; i += 3) {
		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);

		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
		glVertex3f(tris[i + 1].x, tris[i + 1].y, tris[i + 1].z);
		glVertex3f(tris[i + 2].x, tris[i + 2].y, tris[i + 2].z);
		glEnd();
	}
}

/*------------------------- Cone ----------------------------*/

Cone::Cone()
{
	a = 0.f;
	raio = 0.0f;
	lados = 0.0f;
	niveis = 0.0f;
}
void Cone::readfromFile(string filename)
{
	std::fstream file(filename, std::ios_base::in);
	file >> nome;
	file >> a;
	file >> raio;
	file >> lados;
	file >> niveis;

	float x, y, z;
	while (file >> x >> y >> z) {
		Ponto3D t;
		t.x = x; t.y = y; t.z = z;
		tris.push_back(t);
	}
	file.close();
}

void Cone::geraCone(float altura, float raio, float lados, float niveis, string filename)
{
	this->raio = raio;
	this->a = a;
	this->niveis = niveis;
	this->lados = lados;
	writetoFile(filename);
}

/* Escreve um Cone para ficheiro xml */
void Cone::writetoFile(string filename)
{
	ofstream file(filename);

	int i;
	float hstep = (2 * M_PI) / (float)lados;
	float vstep = a / (float)niveis;
	float rstep = raio / (float)niveis;
	float alpha, h;
	h = 0; alpha = 2 * M_PI;

	file << "CONE\n";
	file << a << "\n";
	file << raio << "\n";
	file << lados << "\n";
	file << niveis << "\n";
	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	for (i = 0; i <= lados; i++) {
		file << raio*sinf(alpha) << " " << 0 << " " << raio*cosf(alpha) << "\n";
		alpha -= hstep;
	}

	alpha = 0;

	for (i = 0; i < niveis; i++) {
		alpha = 0;
		file << 0 << " " << a << " " << 0 << "\n";
		for (int j = 0; j <= lados; j++) {
			file << raio*sinf(alpha) << " " << h << " " << raio*cosf(alpha) << "\n";
			alpha += hstep;
		}
		h += vstep;
		raio = raio - rstep;
	}

	file.close();

}

/* Desenha todos os Triangulos no vetor tris que devem formar um Cone */
void Cone::draw()
{
	int n = tris.size();
	int i;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 1);

	for (i = 0; i < n; i++) {
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
	}
	glEnd();
}