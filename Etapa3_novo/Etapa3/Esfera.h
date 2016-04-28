#ifndef Esfera_h
#define Esfera_h

#include "forma.h"
#include <vector>
#include <string>
#include "glew.h"
#include <glut.h>

class Esfera : public Forma
{
public:
	Esfera();
	float r;
	float n;
	float f;
	void draw();
	void criarVBO(string filename);
	void desenharVBO();
	void geraEsfera(float raio, int niveis, int fatias, string filename);
	void writetoFile(string filename);
};

#endif

