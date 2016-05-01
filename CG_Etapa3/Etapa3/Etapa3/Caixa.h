#ifndef Caixa_h
#define Caixa_h

#include "forma.h"
#include <vector>
#include <string>
#include "glew.h"
#include <glut.h>

class Caixa : public Forma
{
public:
	Caixa(){};
	float c, l, a;
	void geraCaixa(float comprimento, float largura, float altura, string filename);
	void draw();
	void writetoFile(string filename);
};

#endif