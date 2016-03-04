/*
*
*Autores : Miguel Guimarães, Miguel Rodrigues, Xavier Rodrigues, Emanuel Fernandes
*Data : 2016 - 03 - 01
*
*/

#ifndef _formas_h_included_
#define _formas_h_included_

#include <vector>
#include <string>

using namespace std;

class Ponto3D
{
public:
	Ponto3D();
	float x, y, z;
	void print() const;
	string toString();
};

class Forma
{
protected:
	virtual ~Forma() {};
	vector<Ponto3D> tris;  // vetor de triangulos da forma
public:
	string nome;
	virtual void readfromFile(string filename);
	virtual void draw() = 0;
	virtual void writetoFile(string filename) = 0;
};

class Triangulo : public Forma
{
public:
	Triangulo();
	float lado;
	void geraTriangulo(float l, string filename);
	void draw();
	void writetoFile(string filename);
};

class Circulo : public Forma
{
public:
	Circulo();
	int lados;
	float raio;
	void geraCirculo(float raio, int lados, string filename);
	void writetoFile(string filename);
	void draw();
};

class Plano : public Forma
{
public:
	Plano();
	float c, l;
	void geraPlano(float comprimento, float largura, string filename);
	void draw();
	void writetoFile(string filename);
};

class Caixa : public Forma
{
public:
	Caixa();
	float c, l, a;
	void geraCaixa(float comprimento, float largura, float altura, string filename);
	void draw();
	void writetoFile(string filename);
};

class Esfera : public Forma
{
public:
	Esfera();
	float r;
	float n;
	float f;
	void geraEsfera(float raio, float niveis, float fatias, string filename);
	void draw();
	void writetoFile(string filename);
};

class Cone : public Forma
{
public:
	Cone();
	float altura;
	float raio;
	float nlados;
	float ncamadas;
	void readfromFile(string filename) override;
	void geraCone(float altura, float raio, int nlados, int ncamadas, string filename);
	void draw();
	void writetoFile(string filename);
};

#endif