#ifndef Circulo_h
#define Circulo_h

#include "const.h"
#include "forma.h"

class Circulo : public Forma
{
public:
	Circulo(){};
	int lados;
	float raio;
	void geraCirculo(float raio, int lados, string filename);
	void writetoFile(string filename);
	void draw();
	virtual void desenharVBO() override;
};

#endif