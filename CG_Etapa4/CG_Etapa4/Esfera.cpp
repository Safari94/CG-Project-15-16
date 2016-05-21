#include "Esfera.h"
#include "const.h"

using namespace std;

Esfera::Esfera()
{
	r = 0.0f;
	n = 0.0f;
	f = 0.0f;
}

void Esfera::geraEsfera(float raio, int niveis, int fatias, string filename) {
	r = raio; n = niveis; f = fatias;
	writetoFile(filename);
}

void Esfera::writetoFile(string filename) {

	int arrayS = 3*(n*n*6);
	int tSize = 2*(n*n*6);

	float* aVertex = (float*)malloc(arrayS*sizeof(float));
	float* aNormal = (float*)malloc(arrayS*sizeof(float));
	float* aTexture = (float*)malloc(tSize*sizeof(float));

	int index = 6 * ((f + n)*n);
	int* Index = (int*)malloc(sizeof(int)*index);

	double divH = (2 * M_PI) / f;
	double divV = (M_PI) / n;
	float angv, angh;

	int pos = 0;
	int tpos = 0;
	for (int i = 0; i <= n; i++){
		angv = divV*i;
		for (int j = 0; j <= f; j++){
			angh = divH*j;

			aVertex[pos] = r*sin(angv)*sin(angh);
			aNormal[pos++] = sin(angv)*sin(angh);

			aVertex[pos] = r*cos(angv);
			aNormal[pos++] = cos(angv);

			aVertex[pos] = r*sin(angv)*cos(angh);
			aNormal[pos++] = sin(angv)*cos(angh);

			aTexture[tpos++] = 1 + ((float)j / (float)(f + 1));
			aTexture[tpos++] = (float)i / (float)(n + 1);
		}
	}

	pos = 0;
	for (int i = 0; i<n; i++){
		for (int j = 0; j<f; j++){
			Index[pos++] = j + (i*(f + 1));
			Index[pos++] = j + ((i + 1)*(f + 1));
			Index[pos++] = (j + 1) + (i*(f + 1));

			Index[pos++] = (j + 1) + (i*(f + 1));
			Index[pos++] = j + ((i + 1)*(f + 1));
			Index[pos++] = (j + 1) + ((i + 1)*(f + 1));
		}
	}

	ofstream file(filename);

	file << "ESFERA\n";
	file << arrayS << "\n";
	for (int i = 0; i < arrayS; i += 3)
		file << aVertex[i] << " " << aVertex[i + 1] << " " << aVertex[i + 2] << " \n";

	file << arrayS << "\n";
	for (int i = 0; i < arrayS; i += 3)
		file << aNormal[i] << " " << aNormal[i + 1] << " " << aNormal[i + 2] << " \n";

	file << tSize << "\n";
	for (int i = 0; i < tSize; i += 2)
		file << aTexture[i] << " " << aTexture[i + 1] << " \n";

	file << index << "\n";
	for (int i = 0; i < index; i += 3){
		file << Index[i] << " " << Index[i + 1] << " " << Index[i + 2] << " \n";
	}

	file.close();

	free(aVertex); free(aNormal);
	free(aTexture); free(Index);
}

void Esfera::draw() {
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
	desenhar_curva();
}




void Esfera::criarVBO(string filename) {
	std::fstream file(filename, std::ios_base::in);
	
	float x, y, z;
	int i = 0;
	int indice = 0;
	int sizeV, sizeN, sizeT;
	float* vertexB, *vertexN, *vertexT;
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	file >> nome;
	
	file >> sizeV;
	vertexB = (float*)malloc((sizeV)* sizeof(float));
	while ( i < sizeV) {
		file >> x >> y >> z;
		vertexB[indice++] = x;
		vertexB[indice++] = y;
		vertexB[indice++] = z;
		i += 3;
	}
	
	file >> sizeN;
	vertexN = (float*)malloc((sizeN)* sizeof(float));
	i = indice = 0;
	while ( i < sizeN) {
		file >> x >> y >> z;
		vertexN[indice++] = x;
		vertexN[indice++] = y;
		vertexN[indice++] = z;
		i += 3;
	}
	
	file >> sizeT;
	vertexT = (float*)malloc((sizeT)* sizeof(float));
	i = indice = 0;
	while ( i < sizeT) {
		file >> x >> y;
		vertexT[indice++] = x;
		vertexT[indice++] = y;
		i += 2;
	}
	
	i = indice = 0;
	file >> vertexCount;
	aIndex = (int*)malloc(sizeof(int)*vertexCount);
	while (i < vertexCount) {
		file >> x >> y >> z;
		aIndex[indice++] = x;
		aIndex[indice++] = y;
		aIndex[indice++] = z;
		i += 3;
	}
	file.close();
	//GLuint [3];


	glGenBuffers(3, buffers);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeV, vertexB, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeN, vertexN, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER,buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeT, vertexT, GL_STATIC_DRAW);
	free(vertexB);
	free(vertexN); 
	free(vertexT);
}

void Esfera::desenharVBO() {
	transformacoes();

	if (textfich.compare("EMPTY") == 0){
		glColor3f(color.x, color.y, color.z);
	}
	
	glBindTexture(GL_TEXTURE_2D, textura) ;

	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glVertexPointer(3,GL_FLOAT,0,0);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
	glNormalPointer(GL_FLOAT,0,0);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[2]);
	glTexCoordPointer(2,GL_FLOAT,0,0);
	
	/* Bind da textura */
	glBindTexture(GL_TEXTURE_2D, textura);
	glDrawElements(GL_TRIANGLES, vertexCount ,GL_UNSIGNED_INT, aIndex);
	/* Unbind da textura */
	glBindTexture(GL_TEXTURE_2D, 0) ;
	

	glPopMatrix();
}
