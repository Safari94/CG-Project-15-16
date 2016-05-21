#include "Triangulo.h"
#include "const.h"

using namespace std;

Triangulo::Triangulo()
{
	lado = 0.0f;
}

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

void Triangulo::desenharVBO() {
	transformacoes();

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glBindTexture(GL_TEXTURE_2D, textura);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

void Triangulo::criarVBO(string filename)
{
	std::fstream file(filename, std::ios_base::in); // Nome do ficheiro supostamente temos de ir buscá-lo a um ficheiro .xml?

	int size;
	file >> size;
	cout << size;
	float* vertexB;

	vertexB = (float*)malloc((size)* sizeof(float));
	int indice = 0;
	glEnableClientState(GL_VERTEX_ARRAY);
	float x, y, z;
	while (file >> x >> y >> z) {
		vertexB[indice++] = x;
		vertexB[indice++] = y;
		vertexB[indice++] = z;
	}
	file.close();
	vertexCount = size;

	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertexB, GL_STATIC_DRAW);

	free(vertexB);
}