#include "Triangulo.h"
#include "const.h"

using namespace std;

void Triangulo::geraTriangulo(float l, string filename) {
	lado = l;
	writetoFile(filename);
}

void Triangulo::writetoFile(string filename) {
	float h = sinf(M_PI / 3)*(lado / 2); // altura
	ofstream file(filename);
	
	file << "TRIANGULO\n";
	file << "18\n";

	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	file << h << " " << 0.0f << " " << 0.0f << "\n";
	file << 0.0f << " " << 0.0f << " " << (-lado / 2) << "\n";
	
	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	file << 0.0f << " " << 0.0f << " " << (lado / 2) << "\n";
	file << h << " " << 0.0f << " " << 0.0f << "\n";
	
	file << "18\n";

	file << 0 << " " << 1 << " " << 0 << "\n";
	file << 0 << " " << 1 << " " << 0 << "\n";
	file << 0 << " " << 1 << " " << 0 << "\n";
	
	file << 0 << " " << 1 << " " << 0 << "\n";
	file << 0 << " " << 1 << " " << 0 << "\n";
	file << 0 << " " << 1 << " " << 0 << "\n";
	
	file << "12\n";

	file << 0.5f << " " << 0 << "\n";
	file << 0.5f << " " << 1 << "\n";
	file << 1 << " " << 0 << "\n";
	
	file << 0.5f << " " << 0 << "\n";
	file << 0 << " " << 0 << "\n";
	file << 0.5f << " " << 1 << "\n";
	
	file.close();
}

/*FunÁ„o que desenhar um conjunto de tri‚ngulos armazenados de um modo sequencial*/
void Triangulo::draw() {
	int n;
	
	transformacoes();
	
	n = tris.size();
	for (int i = 0; i < n; i += 3){
		glBegin(GL_TRIANGLES);
		glColor3f(color.x, color.y, color.z);
		
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
		glVertex3f(tris[i + 1].x, tris[i + 1].y, tris[i + 1].z);
		glVertex3f(tris[i + 2].x, tris[i + 2].y, tris[i + 2].z);
		glEnd();
	}
	
	glPopMatrix();
	
}