#ifndef Cone_h
#define Cone_h

#include "forma.h"
#include "const.h"
#include <vector>
#include <string>
#include "glew.h"
#include <glut.h>

class Cone : public Forma
{
public:
	Cone() {};
	float altura;
	float raio;
	float nlados;
	float ncamadas;
	int *aIndex;
	void readfromFile(string filename) override;
	void geraCone(float altura, float raio, int nlados, int ncamadas, string filename);
	void draw();
	void desenharVBO() override;
	void criarVBO(string filename) override;
	void writetoFile(string filename);
};

#endif