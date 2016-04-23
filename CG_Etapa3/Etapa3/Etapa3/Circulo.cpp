
#include "Circulo.h"
#include "const.h"

using namespace std;

void Circulo::geraCirculo(float raio, int lados, string filename){
	raio=raio; lados=lados;writetoFile(filename);
}

void Circulo::writetoFile(string filename) {
	float alpha = 2 * M_PI;
	float decAngulo = (2 * M_PI) / lados;
	
	int size = (6 + 2 * (lados+1) * 3);
	int tsize = (4 + 2 * (lados+1) * 2);
	int v, t, n;
	v = t = n = 0;
	float texF = 1.0f/lados;

	float *vertexB=(float*)malloc(size*sizeof(float));
	float *normalB=(float*)malloc(size*sizeof(float));
	float *texB=(float*)malloc(tsize*sizeof(float));
	
	vertexB[v++]=0;
	vertexB[v++]=0;
	vertexB[v++]=0;
	
	normalB[n++]=0;
	normalB[n++]=-1;
	normalB[n++]=0;
	
	texB[t++]=1;
	texB[t++]=0;

	for (int i = 0; i <= lados; i++){
		vertexB[v++]=raio*sinf(alpha);
		vertexB[v++]=0;
		vertexB[v++]=raio*cosf(alpha);
		
		normalB[n++]=0;
		normalB[n++]=-1;
		normalB[n++]=0;

		texB[t++]=i*texF;
		texB[t++]=1;

		alpha -= decAngulo;
	}
	
	vertexB[v++]=0;
	vertexB[v++]=0;
	vertexB[v++]=0;
	
	normalB[n++]=0;
	normalB[n++]=1;
	normalB[n++]=0;
	
	texB[t++]=1;
	texB[t++]=0;

	for (int i = 0; i <= lados; i++){
		
		vertexB[v++]=raio*cosf(alpha);
		vertexB[v++]=0;
		vertexB[v++]=raio*sinf(alpha);
		
		normalB[n++]=0;
		normalB[n++]=1;
		normalB[n++]=0;
		
		texB[t++]=i*texF;
		texB[t++]=1;

		alpha -= decAngulo;
	}
	
	ofstream file;
	file.open(filename);
	int i;
	file << "CIRCULO\n";
	file << size << "\n";
	for (i = 0; i < size; i += 3)
		file << vertexB[i] << " " << vertexB[i + 1] << " " << vertexB[i + 2] << " \n";
	
	file << size << "\n";
	for (i = 0; i < size; i += 3)
		file << normalB[i] << " " << normalB[i + 1] << " " << normalB[i + 2] << " \n";
	
	file << tsize << "\n";
	for (i = 0; i < tsize; i += 2)
		file << texB[i] << " " << texB[i + 1] << " \n";
	
	file.close();
	
	free(vertexB);
	free(normalB);
	free(texB);
}

void Circulo::draw() {
	int i, n;
	
	transformacoes();
	
	n = tris.size();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(color.x, color.y, color.z);
	for (i = 0; i <= n; i++){
		glVertex3f(tris[i].x, tris[i].y, tris[i].z);
	}
	glEnd();
	
	glPopMatrix();
	
}

void Circulo::desenharVBO() {
	transformacoes();
	
	
	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glVertexPointer(3,GL_FLOAT,0,0);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
	glNormalPointer(GL_FLOAT,0,0);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[2]);
	glTexCoordPointer(2,GL_FLOAT,0,0);
	
	/* Bind da textura */
	glBindTexture(GL_TEXTURE_2D, textura) ;
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
	/* Unbind da textura */
	glBindTexture(GL_TEXTURE_2D, 0) ;
	
	glPopMatrix();
}