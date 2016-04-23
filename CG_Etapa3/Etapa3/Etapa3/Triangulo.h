#ifndef Triangulo_h
#define Triangulo_h

#include "forma.h"
#include <vector>
#include <string>
#include "glew.h"
#include <glut.h>

class Triangulo : public Forma
{
public:
	Triangulo();
	float lado;
	void geraTriangulo(float l, string filename);
	void draw();
	void writetoFile(string filename);
};


#endif