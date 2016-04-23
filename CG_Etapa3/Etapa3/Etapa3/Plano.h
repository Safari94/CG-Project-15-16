#ifndef Plano_h
#define Plano_h

#include "forma.h"
#include <vector>
#include <string>
#include "glew.h"
#include <glut.h>

class Plano : public Forma
{
public:
	Plano();
	float c, l;
	void geraPlano(float comprimento, float largura, string filename);
	void draw();
	void writetoFile(string filename);
};

#endif