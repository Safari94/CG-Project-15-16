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



