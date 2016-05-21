/*
*	Autores: Daniel Caldas, José Cortez, Marcelo Gonçalves, Ricardo Silva
*	Data: 2015.05.04
*	Versão: 3.0v
*
*/

#ifndef _formas_h_included_
#define _formas_h_included_

#include <vector>
#include <string>
#include <GL/glew.h>
#include <windows.h>


using namespace std;

void init(int nformas);

class Ponto3D {
 public:
	Ponto3D();
	Ponto3D(float xx, float yy, float zz)
	{
		x = xx; y = yy; z = zz;
	}
	float x, y, z;
	void printP() const; // debug
	string tostring();
};

// Uma rotãção 3D é uma extensão de um ponto 3D com o acréscimo de um ângulo
class Rotacao3D : public Ponto3D {
 public:
	Rotacao3D();
	float tempo; // Tempo para se complete a rotação (em segundos)
	float ang; // Ângulo
	float timebase;
	void printR() const; // debug
	void rotate();
};

class Translacao3D {
 public:
	Ponto3D first; // Valores para efetuar primeiro translate
	float tempo; // Tempo que leva a ser completada a translação (em segundos)
	float b; // Ponto da curva
	float timebase;
	vector<Ponto3D> pontos; // Pontos que definem a curva da translação
	void printT();
	void translate();
};

// Um objecto TransformsWrapper representa opcionalmente
class TransformsWrapper {
 public:
	string nome;
	Ponto3D escala;
	Translacao3D translacao;
	Rotacao3D rotacao;
	void printTW() {
		if (nome=="SCALE"){
			escala.printP();
		}
		else if (nome=="ROTATE"){
			rotacao.printR();
		}
		else if (nome=="TRANSLATE"){
			translacao.printT();
		}
	}
};

class Grupo {
 public:
	 Grupo(){};
	 Grupo(vector<TransformsWrapper> fromfather){ transfs = fromfather; }
	 int id; // noção de profundidade no ficheiro
	 vector<TransformsWrapper> transfs;
};

class Forma {
 protected:
	vector<Ponto3D> tgls;
	string nome;
	vector<TransformsWrapper> transforms; // Rotações e Translações

 public:
	 GLuint buffers[1];
	 GLuint vertexCount;
	 string astro;

 public:
	Ponto3D color; // Cor da forma
	virtual ~Forma(){};
	virtual void read3DfromFile(string filename);
	virtual void draw()=0;
	void setTransformacoes(vector<TransformsWrapper> ts);
	void applyTransforms();
	void printAllTransforms();
	void desenhar_curva();
	void criarVBO(string filename);
	void drawVBO();

 protected:
	virtual void write3DtoFile(string filename)=0;
};

class Triangulo : public Forma {
 private:
	 float lado; // comprimento do lado

 public:
	Triangulo(){}
	void gerarTriangulo(float l, string filename);
	void draw();
private:
	void write3DtoFile(string filename);
};

class Circulo : public Forma {
 private:
	float raio;
	int nlados;

 public:
	Circulo(){}
	void gerarCirculo(float raio, int nlados, string filename);
	void draw();
	void drawVBO();
private:
	void write3DtoFile(string filename);
};

class Rectangulo : public Forma {
 private:
	float l; // largura
	float c; // comprimento

 public:
	Rectangulo(){}
	void gerarRectangulo(float largura, float comprimento, string filename);
	void draw();
private:
	void write3DtoFile(string filename);
};

class Paralelepipedo : public Forma {
private:
	float l;
	float c;
	float a;

public:
	Paralelepipedo(){}
	void gerarParalelepipedo(float largura, float comprimento, float altura, string filename);
	void draw();
private:
	void write3DtoFile(string filename);
};

class Esfera : public Forma {
 private:
	float r; //r = raio
	int c;   //c = camadas
	int f;   //f = fatias

 public:
	Esfera(){}
	void gerarEsfera(float raio, int camadas, int fatias, string filename);
	void criarVBO();
	void drawVBO();
	void draw();

 private:
	void write3DtoFile(string filename);
};

class Cone : public Forma {
private:
	float altura;
	float raio;
	int nlados;
	int ncamadas;
public:
	Cone(){}
	void read3DfromFile(string filename) override;
	void gerarCone(float altura, float raio, int nlados, int ncamadas, string filename);
	void draw();
	int * aIndex;
	void criarVBO(string filename);
	void drawVBO();
private:
	void write3DtoFile(string filename);
};


class Teapot : public Forma {
	vector<int> indices;
public:
	Teapot();
	vector<int> getPatch() { return indices; }
	void adicionaIndice(int);
	virtual ~Teapot();
	void draw();
	void criarVBO(string filename);
	void read3DfromFile(string filename) override;
private:
	void write3DtoFile(string filename);
};

void readTeapot(string path);
void initSupBezier(int tess, string nameFile);

#endif