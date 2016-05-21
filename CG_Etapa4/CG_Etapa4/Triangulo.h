#ifndef Triangulo_h
#define Triangulo_h

#include "forma.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

class Triangulo : public Forma
{
public:
	Triangulo();
	float lado;
	void geraTriangulo(float l, string filename);
	void draw();
	void desenharVBO() override;
	void criarVBO(string filename) override;
	void writetoFile(string filename);
};


#endif