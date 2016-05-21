#ifndef Cone_h
#define Cone_h

#include "forma.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

class Cone : public Forma
{
public:
	Cone();
	float altura;
	float raio;
	float nlados;
	float ncamadas;
	void readfromFile(string filename) override;
	void geraCone(float altura, float raio, int nlados, int ncamadas, string filename);
	void draw();
	void writetoFile(string filename);
	void criarVBO(string filename);
	void desenharVBO();
};

#endif