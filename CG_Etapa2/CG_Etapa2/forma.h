/*
*
*Autores : Miguel Guimarães, Miguel Rodrigues, Xavier Rodrigues, Emanuel Fernandes
*Data : 2016 - 03 - 29
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

class Rotacao : public Ponto3D {
public:
	Rotacao();
	float ang;
	void printRotacao() const; 
};

class TransformsWrapper {
public:
	string nome;
	Ponto3D escala;
	Ponto3D translacao;
	Rotacao rotacao;
	void printT() const {
		if (nome == "SCALE") {
			escala.print();
		}
		else if (nome == "ROTATE") {
			rotacao.printRotacao();
		}
		else if (nome == "TRANSLATE") {
			translacao.print();
		}
	}
};

class Grupo {
public:
	Grupo() {};
	Grupo(vector<TransformsWrapper> fromfather) { transfs = fromfather; }
	int id; // noção de profundidade no ficheiro
	vector<TransformsWrapper> transfs;
};



class Forma
{
protected:
	virtual ~Forma() {};
	vector<Ponto3D> tris;  // vetor de triangulos da forma
	vector<TransformsWrapper> transforms; // Rotações e Translações
public:
	Ponto3D color; // Cor da forma
	string nome;
	virtual void readfromFile(string filename);
	virtual void draw() = 0;
	void setTransformacoes(vector<TransformsWrapper> ts);
	void transformacoes();
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