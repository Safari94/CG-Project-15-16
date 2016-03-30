/*
*
*Autores : Miguel Guimarães, Miguel Rodrigues, Xavier Rodrigues, Emanuel Fernandes
*Data : 2016 - 03 - 29
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

/*Classe responsavel por fazer a rotaçao*/
Rotacao::Rotacao() {
	ang = 0.0f; x = 0.0f; y = 0.0f; z = 0.0f;
}

void Rotacao::printRotacao() const
{
	cout << "Angulo:" << ang << " X:" << x << " Y:" << y << " Z:" << z << "\n";
}

/*-------------------------- Forma ----------------------------*/

/* Função para ler Pontos de um ficheiro */
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

void Forma::setTransformacoes(vector<TransformsWrapper> ts)
{
	transforms = ts;
}

/**Método que aplica transformações geométricas às formas (método genérico)*/
void Forma::transformacoes()
{
	int i, n;

	n = transforms.size();
	glPushMatrix();
	for (i = 0; i < n; i++) {
		TransformsWrapper tw = transforms.at(i);
		const char* op = tw.nome.c_str();

		if (strcmp(op, "ROTATE") == 0) {
			glRotatef(tw.rotacao.ang, tw.rotacao.x, tw.rotacao.y, tw.rotacao.z);
		}
		else if (strcmp(op, "TRANSLATE") == 0) {
			glTranslatef(tw.translacao.x, tw.translacao.y, tw.translacao.z);
		}
		else if (strcmp(op, "SCALE") == 0) {
			glScalef(tw.escala.x, tw.escala.y, tw.escala.z);
		}
	}
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
	transformacoes();
	for (int i = 0; i < n; i += 3) {
		glBegin(GL_TRIANGLES);
		glColor3f(color.x, color.y, color.z);

		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
		glVertex3f(tris[i + 1].x, tris[i + 1].y, tris[i + 1].z);
		glVertex3f(tris[i + 2].x, tris[i + 2].y, tris[i + 2].z);
		glEnd();
	}
	glPopMatrix();
	return;
}

/*------------------------- Círculo ----------------------------*/

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
	transformacoes();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(color.x, color.y, color.z);

	for (i = 0; i < n; i++) {
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
	}

	glEnd();
	glPopMatrix();
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
	transformacoes();

	glBegin(GL_TRIANGLES);
	glColor3f(color.x, color.y, color.z);

	for (int i = 0; i < n; i++)
	{
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
	}

	glEnd();
	glPopMatrix();

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
void Caixa::draw(){
	transformacoes();
	int n = tris.size();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(color.x, color.y, color.z);

	for (int i = 0; i < n; i++) {
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
	}

	glEnd();
	glPopMatrix();
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
	ofstream file;
	file.open(filename);
	file << "ESFERA\n";
	int i, j;
	double passoH = (2 * M_PI) / f;
	double passoV = (M_PI) / n;
	double altura = r* sin((M_PI / 2) - passoV);
	double alturaCima = r;

	for (i = 0; i < f; i++) {

		double actualX = r*sin(i*passoH);
		double actualZ = r*cos(i*passoH);
		double nextX = r*sin((i + 1)*passoH);
		double nextZ = r*cos((i + 1)*passoH);
		double actX, actZ, nexX, nexZ, cimaActX, cimaActZ, cimaNexX, cimaNexZ;

		for (j = 1; j < n + 2; j++){


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
			altura = r* sin((M_PI / 2) - (passoV*j));

		}

		altura = r* sin((M_PI / 2) - passoV);
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
	transformacoes();
	for (int i = 0; i < n; i += 3) {
		glBegin(GL_TRIANGLES);
		glColor3f(color.x, color.y, color.z);

		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
		glVertex3f(tris[i + 1].x, tris[i + 1].y, tris[i + 1].z);
		glVertex3f(tris[i + 2].x, tris[i + 2].y, tris[i + 2].z);
		glEnd();
	}
	glPopMatrix();
}

/*------------------------- Cone ----------------------------*/

Cone::Cone()
{
	altura = 0.f;
	raio = 0.0f;
	nlados = 0.0f;
	ncamadas = 0.0f;
}
void Cone::geraCone(float altura, float raio, int nlados, int ncamadas, string filename)
{
	this->raio = raio; 
	this->altura = altura;
	this->nlados = nlados;
	this->ncamadas = ncamadas; 
	writetoFile(filename);
}


void Cone::readfromFile(string filename)
{
	std::fstream file(filename, std::ios_base::in); // Nome do ficheiro supostamente temos de ir buscá-lo a um ficheiro .xml?
	

	file >> nome;
	file >> altura;
	file >> raio;
	file >> nlados;
	file >> ncamadas;
	float x, y, z;
	while (file >> x >> y >> z) {
		Ponto3D t;
		t.x = x; t.y = y; t.z = z;
		tris.push_back(t);
	}
	file.close();
}

/*Função que internamente à classe gera o ficheiro os triângulos*/
void Cone::writetoFile(string filename)
{
	ofstream file;
	file.open(filename);

	int i;
	float incAngulo = (2 * M_PI) / (float)nlados;
	float incAltura = altura / (float)ncamadas;
	float incRaio = raio / (float)ncamadas;
	float alpha, h;
	h = 0; alpha = 2 * M_PI;

	file << "CONE\n";
	file << altura << "\n";
	file << raio << "\n";
	file << nlados << "\n";
	file << ncamadas << "\n";
	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	for (i = 0; i <= nlados; i++) {
		file << raio*sinf(alpha) << " " << 0 << " " << raio*cosf(alpha) << "\n";
		alpha -= incAngulo;
	}

	alpha = 0;

	for (i = 0; i < ncamadas; i++) {
		alpha = 0;
		file << 0 << " " << altura << " " << 0 << "\n";
		for (int j = 0; j <= nlados; j++) {
			file << raio*sinf(alpha) << " " << h << " " << raio*cosf(alpha) << "\n";
			alpha += incAngulo;
		}
		h += incAltura;
		raio = raio - incRaio;
	}

	file.close();
}


void Cone::draw()
{
	int n = tris.size();
	int i;
	transformacoes();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(color.x, color.y, color.z);
	for (i = 0; i <= nlados + 1; i++) {
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
	}
	glEnd();

	for (; i < n;) {
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(color.x, color.y, color.z);
		glVertex3f(tris[i].x, tris[i].y, tris[i].z); i++;
		for (int j = 0; j <= nlados; j++) {
			glVertex3f(tris[i].x, tris[i].y, tris[i].z); i++;
		}
		glEnd();
	}
	glPopMatrix();
}