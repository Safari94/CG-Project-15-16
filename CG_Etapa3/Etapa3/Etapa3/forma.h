/*
*
*Autores : Miguel Guimarães, Miguel Rodrigues, Xavier Rodrigues, Emanuel Fernandes
*Data : 2016 - 03 - 29
*
*/
#include "const.h"
#ifndef _formas_h_included_
#define _formas_h_included_

#include <vector>
#include <string>

using namespace std;

void iniciar(int nformas);

class Ponto3D
{
public:
	Ponto3D();
	Ponto3D(float xx, float yy, float zz)
	{
	x = xx; y = yy; z = zz;
	}
	float x, y, z;
	void print() const;
	string toString();
};

class Rotacao : public Ponto3D {
public:
	Rotacao();
	float tempo;
	float timebase;
	float ang;
	void printRotacao() const; 
	void rodar();
};

class Translacao : public Ponto3D {
public:
	Ponto3D fst; // valores para efetuar o primeiro translate
	float tempo; // tempo que demorara essa tranalsaçao
	float b;
	float timebase;
	vector<Ponto3D> pontos;
	void printTranslacao() const;
	void translacao();

};

class TransformsWrapper {
public:
	string nome;
	Ponto3D escala;
	Translacao translacao;
	Rotacao rotacao;
	void printT() const {
		if (nome == "SCALE") {
			escala.print();
		}
		else if (nome == "ROTATE") {
			rotacao.printRotacao();
		}
		else if (nome == "TRANSLATE") {
			translacao.printTranslacao();
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
	string nome;
	vector<Ponto3D> tris;  // vetor de triangulos da forma
	vector<TransformsWrapper> transforms; // Rotações e Translações
public:
	GLuint buffers[3],vertexCount;
	int *aIndex;
	unsigned int textura ;
	string astro;

public:
	virtual ~Forma() {};
	Ponto3D color; // Cor da forma
	
	virtual void readfromFile(string filename);
	virtual void draw() = 0;
	void setTransformacoes(vector<TransformsWrapper> ts);
	void transformacoes();
	void printTransformacoes() const;
	virtual void writetoFile(string filename) = 0;
	
	void desenhar_curva();
	virtual void criarVBO(string filename);
	virtual void desenharVBO();
	void carregarImagem();
};


#endif