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
	void geraEsfera(float raio, float niveis, float fatias, string filename);
	void draw();
	void writetoFile(string filename);
};

#endif

