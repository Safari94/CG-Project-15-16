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
#include <glew.h>
#include <glut.h>

using namespace std;

void iniciar(int nformas);

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
	float tempo;
	float timebase;
	float ang;
	void printRotacao() const; 
	void rodar();
};

class Translacao{
public:
	Ponto3D fst; // valores para efetuar o primeiro translate
	float tempo; // tempo que demorara essa tranalsaçao
	float b;
	float timebase;
	vector<Ponto3D> pts;
	void translacao();

	void printTranslacao() const;

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
	virtual ~Forma() {};
	vector<Ponto3D> tris;  // vetor de triangulos da forma
	vector<TransformsWrapper> transforms; // Rotações e Translações
public:
	GLuint buffers[3];
	GLuint vertexCount;
	int *aIndex;
	unsigned int textura ;
	string astro;

public:
	Ponto3D color; // Cor da forma
	string nome;
	virtual void readfromFile(string filename);
	virtual void draw() = 0;
	void setTransformacoes(vector<TransformsWrapper> ts);
	void transformacoes();
	void printAllTransforms();
	void printTransformacoes();
	virtual void writetoFile(string filename) = 0;
	void desenharTransformacoes();
	void desenhar_curva();
	virtual void criarVBO(string filename);
	virtual void desenharVBO();
	void carregarImagem();
};


#endif