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

class Translacao{
public:
	Ponto3D fst; // valores para efetuar o primeiro translate
	float time; // tempo que demorara essa tranalsaçao
	float p;
	float tb;
	vetor<Ponto3D> pts;
	void printTranslacao();
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
	virtual void writetoFile(string filename) = 0;
	void desenharTransformacoes();
	void desenhar_curva();
	virtual void criarVBO(string filename);
	virtual void desenharVBO();
	void carregarImagem();
};


#endif