#ifndef Caixa_h
#define Caixa_h

#include "forma.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

class Caixa : public Forma
{
public:
	Caixa();
	float c, l, a;
	void geraCaixa(float comprimento, float largura, float altura, string filename);
	void draw();
	void desenharVBO() override;
	void criarVBO(string filename) override;
	void writetoFile(string filename);
};

#endif