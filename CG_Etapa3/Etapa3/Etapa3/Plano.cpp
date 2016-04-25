#include "Plano.h"
#include "const.h"

using namespace std;

void Plano::geraPlano(float largura, float comprimento, string filename){
	l = largura;
	c = comprimento;
	writetoFile(filename);
}

void Plano::writetoFile(string filename) {
	std::ofstream file;
	file.open(filename);
	
	//Imprimir os vertices, indices, normais e coordenadas de textura
	
	file << "Plano" << "\n";
	file << 36 << "\n";
	
	file << -l / 2.0f << " " << 0.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << 0.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << 0.0f << " " << -c / 2.0f << "\n";
	
	file << -l / 2.0f << " " << 0.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << 0.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << 0.0f << " " << -c / 2.0f << "\n";
	
	file << c / 2.0f << " " << 0.0f << " " << -l / 2.0f << "\n";
	file << -c / 2.0f << " " << 0.0f << " " << l / 2.0f << "\n";
	file << -c / 2.0f << " " << 0.0f << " " << -l / 2.0f << "\n";
	
	file << c / 2.0f << " " << 0.0f << " " << -l / 2.0f << "\n";
	file << c / 2.0f << " " << 0.0f << " " << l / 2.0f << "\n";
	file << -c / 2.0f << " " << 0.0f << " " << l / 2.0f << "\n";
	
	file << 36 << "\n";
	
	file << 0 << " " << 1 << " " << 0 << "\n";
	file << 0 << " " << 1 << " " << 0 << "\n";
	file << 0 << " " << 1 << " " << 0 << "\n";
	
	file << 0 << " " << 1 << " " << 0 << "\n";
	file << 0 << " " << 1 << " " << 0 << "\n";
	file << 0 << " " << 1 << " " << 0 << "\n";
	
	file << 0 << " " << -1 << " " << 0 << "\n";
	file << 0 << " " << -1 << " " << 0 << "\n";
	file << 0 << " " << -1 << " " << 0 << "\n";
	
	file << 0 << " " << -1 << " " << 0 << "\n";
	file << 0 << " " << -1 << " " << 0 << "\n";
	file << 0 << " " << -1 << " " << 0 << "\n";
	
	file << 24 << "\n";
	
	file << 0 << " " << 1 << "\n";
	file << 1 << " " << 0 << "\n";
	file << 0 << " " << 0 << "\n";
	
	file << 0 << " " << 1 << "\n";
	file << 1 << " " << 1 << "\n";
	file << 1 << " " << 0 << "\n";
	
	file << 1 << " " << 0 << "\n";
	file << 0 << " " << 1 << "\n";
	file << 1 << " " << 1 << "\n";
	
	file << 1 << " " << 0 << "\n";
	file << 0 << " " << 0 << "\n";
	file << 0 << " " << 1 << "\n";
	
	file.close();
}

	void Plano::draw() {
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

