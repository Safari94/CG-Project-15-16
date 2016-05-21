/*
*	Autores: Daniel Caldas, José Cortez, Marcelo Gonçalves, Ricardo Silva
*	Data: 2015.05.04
*	Versão: 3.0v
*
*/

#include "formas.h"
#include "const.h"

using namespace std;

# define M_PI 3.14159265358979323846 /*Pi*/

// Variáveis globais para armazenamento de tempos e ângulos relativos a animações
int N; // número total de figuras a desenhar numa cena

float *global_b; // Array de tempos globais para cada figura
int global_index; // Índice da figura atual que se está a desenhar

float *global_timeb;
float *global_timer;
float *global_ang;

/**Funções que cada sólido herda diretamente da super classe sólido
ou implementa uma versão s:

- write3DtoFile(string filename): gera pontos automaticamente armazenando-os diretamente num ficheiro;

- read3DfromFile(string filename): lê um conjunto de pontos previamente gerado pela aplicação armazenando-os
na estrutura de dados adequada;

- draw(): método da forma implementado em conformidade que é usado para desenhar a cena no ciclo glut.
*/


/*Método inicializar*/
void init(int nformas)
{
	N = nformas;

	global_b = (float*)malloc(N * sizeof(float));
	global_timeb = (float*)malloc(N * sizeof(float));
	global_ang = (float*)malloc(N * sizeof(float));
	global_timer = (float*)malloc(N * sizeof(float));

	global_index = 0;

	for (int i = 0; i < N; i++){
		global_b[i] = 0;
		global_timeb[i] = 0;
		global_ang[i] = 360; // Ângulo de translação de um planeta
		global_timer[i] = 0;
	}
}

/*------------ Funções auxiliares para desenhar Catmull-Rom Curves ---------------*/

void getCatmullRomPoint(float t, int *indices, float *res, float p[][3]) {
	int i;
	float res_aux[4];
	// catmull-rom matrix
	float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },
	{ 1.0f, -2.5f, 2.0f, -0.5f },
	{ -0.5f, 0.0f, 0.5f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f } };

	// Calcular o ponto res = T * M * P
	// sendo Pi = p[indices[i]]

	//Sem derivada
	for (i = 0; i<4; i++)
		res_aux[i] = pow(t, 3) * m[0][i] + pow(t, 2) * m[1][i] + t * m[2][i] + m[3][i];

	//Calculo do RES
	for (i = 0; i<3; i++){
		res[i] = res_aux[0] * p[indices[0]][i] + res_aux[1] * p[indices[1]][i] + res_aux[2] * p[indices[2]][i] + res_aux[3] * p[indices[3]][i];
	}
}

// Dado o t global, retorna o ponto na curva
void getGlobalCatmullRomPoint(float gt, float *res, float p[][3], int pc) {

	float t = gt * pc; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + pc - 1) % pc;	indices[1] = (indices[0] + 1) % pc;
	indices[2] = (indices[1] + 1) % pc; indices[3] = (indices[2] + 1) % pc;

	getCatmullRomPoint(t, indices, res, p);
}

void renderCatmullRomCurve(float p[][3], int pc) {
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < pc; i++){
		glVertex3f(p[i][0],p[i][1],p[i][2]);
	}
	glEnd();
}

/*---------------------------------------------------------------------*/


/*------------------ Class - Translacao3D -----------------------------*/
void Translacao3D::translate()
{
	if (tempo == 0){
		glTranslatef(first.x, first.y, first.z);
	}
	else {
		float res[3];
		int pc = pontos.size();
		float p1[100][3];
		// prencher com os pontos de control
		for (int i = 0; i < pc; i++){
			p1[i][0] = pontos[i].x;
			p1[i][1] = pontos[i].y;
			p1[i][2] = pontos[i].z;	
		}

		renderCatmullRomCurve(p1, pc);
		getGlobalCatmullRomPoint(global_b[global_index], res, p1, pc);

		glTranslatef(res[0], res[1], res[2]);
		
		float inc = (0.025) / tempo;

		// 0.025 é o incremento que temos que dar para percorrer o caminho em 1 seg
		// Sabemos que o valor máximo de t vai ser 1 logo para 1 seg precisamos de incrementar 1
		// é facil concluir que para 25 ms precisa de incrementar 0.025 (regra de 3 simples)
		float time = glutGet(GLUT_ELAPSED_TIME);
		if (time - global_timeb[global_index] >= 25){
			// Assim executa-se de 25 em 25 ms para não haver problemas com pcs mais fracos
			global_timeb[global_index] = time;
			global_b[global_index] += inc;
		}
		if (global_index == N-1) global_index = 0;
		else global_index++;
	}
}

void Translacao3D::printT()
{
	cout << "TRANSLACAO\n";
	first.printP();
	cout << "B: " << b << "\n";
	for (int i = 0; i < pontos.size(); i++){
		pontos[i].printP();
	}
	cout << "Tempo: " << tempo << "\n";
}

/*---------------------------------------------------------------------*/


/*--------------------- Class - Rotacao3D -----------------------------*/
Rotacao3D::Rotacao3D()
{
	tempo = 0.0f;  x = 0.0f; y = 0.0f; z = 0.0f;
}

void Rotacao3D::printR() const
{
	cout << "ROTACAO\n" << "Tempo:" << tempo << "Angulo: " << ang << " X:" << x << " Y:" << y << " Z:" << z << "\n";
}

void Rotacao3D::rotate()
{
	if (tempo == 0){
		glRotatef(global_ang[global_index], x, y, z);
	}
	else {
		glRotatef(global_ang[global_index], x, y, z);
		float inc = (9) / tempo;

		// 9 é o valor do incremento para se percorrer o caminho em 1 seg, facilmente vemos isso
		// porque se precisa de 1 seg para incrementar 360 graus logo é necessário incrementar 9 a cada 25 ms 
		float time = glutGet(GLUT_ELAPSED_TIME);
		if (time - global_timer[global_index] > 25){ // assim executa de 25 em 25 ms 
			global_timer[global_index] = time;
			global_ang[global_index] += inc;
		}
	}
}
/*---------------------------------------------------------------------*/


/*---------------------- Class - Ponto3D -----------------------------*/
Ponto3D::Ponto3D()
{
	x = 0.0f; y = 0.0f; z = 0.0f;
}

/**Imprime ponto3d para stdout*/
void Ponto3D::printP() const
{
	cout << "X: " << x << " Y: " << y <<  "Z: " << z << "\n";
}

/**Converte ponto3d em string*/
string Ponto3D::tostring()
{
	stringstream ss;
	ss << x << " " << y << " " << z;
	return ss.str();
}
/*---------------------------------------------------------------------*/


/*---------------------- Class - Forma -------------------------------*/

/**Função que lê um conjunto de triângulos de um dado ficheiro, cada subclasse poderá
eventualmente ter a necessidade de implementar a sua função de leitura mas esta é a base*/
void Forma::read3DfromFile(string filename)
{
	std::fstream file(filename, std::ios_base::in); // Nome do ficheiro supostamente temos de ir buscá-lo a um ficheiro .xml?

	file >> nome;
	float x, y, z;
	while (file >> x >> y >> z)
	{
		Ponto3D t;
		t.x = x; t.y = y; t.z = z;
		tgls.push_back(t); // Adicionar triangulo ao vector de triangulos da esfera
	}
	file.close();
}

void Forma::setTransformacoes(vector<TransformsWrapper> ts)
{
	transforms = ts;
}

/**Método que aplica transformações geométricas às formas (método genérico)*/
void Forma::applyTransforms()
{
	int i, n;

	n = transforms.size();
	glPushMatrix();
	for (i = 0; i < n; i++){
		TransformsWrapper tw = transforms[i];
		const char* op = tw.nome.c_str();

		if (strcmp(op, "ROTATE") == 0){
			tw.rotacao.rotate();
		}
		else if (strcmp(op, "TRANSLATE") == 0){
			tw.translacao.translate();
		}
		else if (strcmp(op, "SCALE") == 0){
			glScalef(tw.escala.x, tw.escala.y, tw.escala.z);
		}
	}
}

/*Imprimir todas as transformações para debug*/
void Forma::printAllTransforms()
{
	int i;
	int n = transforms.size();

	for (i = 0; i < n; i++){
		transforms.at(i).printTW();
	}
}

/*Função que desenhar curva (trajetória elíptica de um planeta)*/
void Forma::desenhar_curva()
{
	int n = transforms.size();
	for (int i = 0; i < n; i++){
		TransformsWrapper tw = transforms[i];

		if (tw.nome.compare("TRANSLATE") == 0){
			float res[3];
			int pc = tw.translacao.pontos.size();
			float p1[100][3];
			// prencher com os pontos de control
			for (int i = 0; i < pc; i++){
				p1[i][0] = tw.translacao.pontos[i].x;
				p1[i][1] = tw.translacao.pontos[i].y;
				p1[i][2] = tw.translacao.pontos[i].z;
			}

			renderCatmullRomCurve(p1, pc);
		}
	}
}

void Forma::criarVBO(string filename) {
	std::fstream file(filename, std::ios_base::in);

	file >> nome;
	int size;
	file >> size;
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

void Forma::drawVBO() {
	applyTransforms();
	glColor3f(color.x, color.y, color.z);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glPopMatrix();
}
/*---------------------------------------------------------------------*/


/*-------------- Class - Triângulo (Equilátero) -----------------------*/
void Triangulo::gerarTriangulo(float l, string filename)
{
	lado = l;
	write3DtoFile(filename);
}

void Triangulo::write3DtoFile(string filename)
{
	float h = sinf(M_PI / 3)*(lado / 2); // altura
	ofstream file(filename);

	file << "TRIANGULO\n";

	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	file << h << " " << 0.0f << " " << 0.0f << "\n";
	file << 0.0f << " " << 0.0f << " " << (-lado / 2) << "\n";

	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	file << 0.0f << " " << 0.0f << " " << (lado / 2) << "\n";
	file << h << " " << 0.0f << " " << 0.0f << "\n";

	file.close();
}

/*Função que desenhar um conjunto de triângulos armazenados de um modo sequencial*/
void Triangulo::draw()
{
	int n;
	
	applyTransforms();	

	n = tgls.size();
	for (int i = 0; i < n; i += 3){
		glBegin(GL_TRIANGLES);
		glColor3f(color.x, color.y, color.z);

		glVertex3f(tgls[i].x, tgls[i].y, tgls[i].z);
		glVertex3f(tgls[i + 1].x, tgls[i + 1].y, tgls[i + 1].z);
		glVertex3f(tgls[i + 2].x, tgls[i + 2].y, tgls[i + 2].z);
		glEnd();
	}
	
	glPopMatrix();
	
}
/*---------------------------------------------------------------------*/


/*------------------------- Class - Círculo ----------------------------*/
void Circulo::gerarCirculo(float r, int l, string filename)
{
	raio = r; nlados = l;
	write3DtoFile(filename);
}

void Circulo::write3DtoFile(string filename)
{
	float alpha = 2 * M_PI;
	float decAngulo = (2 * M_PI) / nlados;
	ofstream file(filename);

	file << "CIRCULO\n";
	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	for (int i = 0; i <= nlados; i++){
		file << raio*sinf(alpha) << " " << 0 << " " << raio*cosf(alpha) << "\n";
		alpha -= decAngulo;
	}
	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	for (int i = 0; i <= nlados; i++){
		file << raio*cosf(alpha) << " " << 0 << " " << raio*sinf(alpha) << "\n";
		alpha -= decAngulo;
	}

	file.close();
}

void Circulo::draw()
{
	int i, n;
	
	applyTransforms();	

	n = tgls.size();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(color.x, color.y, color.z);
	for (i = 0; i <= n; i++){
		glVertex3f(tgls[i].x, tgls[i].y, tgls[i].z);
	}
	glEnd();
	
	glPopMatrix();
	
}

void Circulo::drawVBO() {
	applyTransforms();
	glColor3f(color.x, color.y, color.z);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

	glPopMatrix();
}
/*---------------------------------------------------------------------*/



/*-------------------------- Class - Plano ----------------------------*/
void Rectangulo::gerarRectangulo(float largura, float comprimento, string filename)
{
	l = largura; c = comprimento;
	write3DtoFile(filename);
}

/*Função que internamente à classe gera o ficheiro os triângulos*/
void Rectangulo::write3DtoFile(string filename) {
	ofstream file;
	file.open(filename);

	file << "RECTANGULO\n";

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


	file.close();
}

/*Função que desenhar um conjunto de triângulos armazenados de um modo sequencial*/
void Rectangulo::draw()
{
	int n;
	
	applyTransforms();	

	n = tgls.size();
	for (int i = 0; i < n; i += 3){
		glBegin(GL_TRIANGLES);
		glColor3f(color.x, color.y, color.z);

		glVertex3f(tgls[i].x, tgls[i].y, tgls[i].z);
		glVertex3f(tgls[i + 1].x, tgls[i + 1].y, tgls[i + 1].z);
		glVertex3f(tgls[i + 2].x, tgls[i + 2].y, tgls[i + 2].z);
		glEnd();
	}	
	
	glPopMatrix();
	
}
/*---------------------------------------------------------------------*/


/*---------------------- Class - Paralelepípedo -----------------------*/
/* Função vísivel do exterior para gerar um ficheiro com triângulos que definem o paralelepípedo */
void Paralelepipedo::gerarParalelepipedo(float largura, float comprimento, float altura, string filename)
{
	l = largura; c = comprimento; a = altura;
	write3DtoFile(filename);
}

/*Função que internamente à classe gera o ficheiro os triângulos*/
void Paralelepipedo::write3DtoFile(string filename) {
	ofstream file;
	file.open(filename);

	file << "PARALELEPIPEDO\n";

	//topo
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";

	file << l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";

	//base
	file << l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";

	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";

	//frente
	file << -l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";

	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";

	//atras
	file << -l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";

	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";

	//lado direito
	file << -l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";

	file << -l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << -l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";

	//lado esquerdo
	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << -c / 2.0f << "\n";
	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";

	file << l / 2.0f << " " << a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << c / 2.0f << "\n";
	file << l / 2.0f << " " << -a / 2.0f << " " << -c / 2.0f << "\n";

	file.close();
}

void Paralelepipedo::draw()
{
	int n;

	
	applyTransforms();	

	n = tgls.size();
	for (int i = 0; i < n; i += 3){
		glBegin(GL_TRIANGLES);
		glColor3f(color.x, color.y, color.z);

		glVertex3f(tgls[i].x, tgls[i].y, tgls[i].z);
		glVertex3f(tgls[i + 1].x, tgls[i + 1].y, tgls[i + 1].z);
		glVertex3f(tgls[i + 2].x, tgls[i + 2].y, tgls[i + 2].z);
		glEnd();
	}
	
	glPopMatrix();
	
}
/*---------------------------------------------------------------------*/


/*---------------------- Class - Esfera -------------------------------*/
/*Função vísivel do exterior para gerar um ficheiro com triângulos que definem a esfera*/
void Esfera::gerarEsfera(float raio, int camadas, int fatias, string filename)
{
	r = raio; c = camadas; f = fatias;
	write3DtoFile(filename);
}

/*Função que internamente à classe gera o ficheiro os triângulos*/
void Esfera::write3DtoFile(string filename)
{
	ofstream file;
	file.open(filename);

	file << "ESFERA\n";

	int size = (f*(c + 1) * 18);
	file << size << "\n";

	int i, j;
	double passoH = (2 * M_PI) / f;
	double passoV = (M_PI) / c;
	double altura = r* sin((M_PI / 2) - passoV);
	double alturaCima = r;

	for (i = 0; i < f; i++) {

		double actualX = r*sin(i*passoH);
		double actualZ = r*cos(i*passoH);
		double nextX = r*sin((i + 1)*passoH);
		double nextZ = r*cos((i + 1)*passoH);
		double actX, actZ, nexX, nexZ, cimaActX, cimaActZ, cimaNexX, cimaNexZ;

		for (j = 1; j < c + 2; j++){


			double aux = cos(asin(altura / r));
			actX = actualX * aux;
			actZ = actualZ * aux;
			nexX = nextX * aux;
			nexZ = nextZ * aux;

			aux = cos(asin(alturaCima / r));
			cimaActX = actualX * aux;
			cimaActZ = actualZ * aux;
			cimaNexX = nextX * aux;
			cimaNexZ = nextZ * aux;

			file << actX << " " << altura << " " << actZ << "\n";
			file << cimaActX << " " << alturaCima << " " << cimaActZ << "\n";
			file << nexX << " " << altura << " " << nexZ << "\n";

			file << cimaActX << " " << alturaCima << " " << cimaActZ << "\n";
			file << cimaNexX << " " << alturaCima << " " << cimaNexZ << "\n";
			file << nexX << " " << altura << " " << nexZ << "\n";

			alturaCima = altura;
			altura = r* sin((M_PI / 2) - (passoV*j));

		}

		altura = r* sin((M_PI / 2) - passoV);
		alturaCima = r;

		actualX = nextX;
		actualZ = nextZ;

	}
	file.close();
}

void Esfera::draw()
{
	int n;

	applyTransforms();

	n = tgls.size();
	for (int i = 0; i < n; i += 3){
		glColor3f(color.x, color.y, color.z);
		glBegin(GL_TRIANGLES);

		glVertex3f(tgls[i].x, tgls[i].y, tgls[i].z);
		glVertex3f(tgls[i + 1].x, tgls[i + 1].y, tgls[i + 1].z);
		glVertex3f(tgls[i + 2].x, tgls[i + 2].y, tgls[i + 2].z);
		glEnd();
	}

	glPopMatrix();
	desenhar_curva();
}
/*---------------------------------------------------------------------*/


/*------------------------ Class - Cone -------------------------------*/
/*Função vísivel do exterior para gerar um ficheiro com triângulos que definem a esfera*/
void Cone::gerarCone(float altura, float raio, int nlados, int ncamadas, string filename)
{
	this->raio = raio; this->altura = altura; this->ncamadas = ncamadas; this->nlados = nlados;
	write3DtoFile(filename);
}


void Cone::read3DfromFile(string filename)
{
	std::fstream file(filename, std::ios_base::in); // Nome do ficheiro supostamente temos de ir buscá-lo a um ficheiro .xml?
	bool flag = false;

	file >> nome;
	file >> altura;
	file >> raio;
	file >> nlados;
	file >> ncamadas;
	float x, y, z;
	while (file >> x >> y >> z){
		Ponto3D t;
		t.x = x; t.y = y; t.z = z;
		tgls.push_back(t);
	}
	file.close();
}

/*Função que internamente à classe gera o ficheiro os triângulos*/
void Cone::write3DtoFile(string filename)
{
	ofstream file;
	file.open(filename);

	int i;
	float incAngulo = (2 * M_PI) / (float)nlados;
	float incAltura = altura / (float)ncamadas;
	float incRaio = raio / (float)ncamadas;
	float alpha, h;
	h = 0; alpha = 2 * M_PI;

	file << "CONE\n";
	file << altura << "\n";
	file << raio << "\n";
	file << nlados << "\n";
	file << ncamadas << "\n";
	file << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
	for (i = 0; i <= nlados; i++){
		file << raio*sinf(alpha) << " " << 0 << " " << raio*cosf(alpha) << "\n";
		alpha -= incAngulo;
	}

	alpha = 0;

	for (i = 0; i < ncamadas; i++){
		alpha = 0;
		file << 0 << " " << altura << " " << 0 << "\n";
		for (int j = 0; j <= nlados; j++) {
			file << raio*sinf(alpha) << " " << h << " " << raio*cosf(alpha) << "\n";
			alpha += incAngulo;
		}
		h += incAltura;
		raio = raio - incRaio;
	}

	file.close();
}


void Cone::draw()
{
	int n;
	int i;
	
	applyTransforms();	

	n = tgls.size();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(color.x, color.y, color.z);
	for (i = 0; i <= nlados+1; i++){
		glVertex3f(tgls[i].x, tgls[i].y, tgls[i].z);
	}
	glEnd();

	for (; i < n;){
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(color.x, color.y, color.z);
		glVertex3f(tgls[i].x, tgls[i].y, tgls[i].z); i++;
		for(int j = 0; j <= nlados; j++) {
			glVertex3f(tgls[i].x, tgls[i].y, tgls[i].z); i++;
		}
		glEnd();
	}
	
	glPopMatrix();
}

void Cone::criarVBO(string filename) {
	std::fstream file(filename, std::ios_base::in);
	file >> nome;
	int size;
	file >> size;
	float* vertexB;

	vertexB = (float*)malloc((size)* sizeof(float));
	int indice = 0;
	glEnableClientState(GL_VERTEX_ARRAY);
	float x, y, z;
	int i = 0;
	cout << size;
	while (i < size) {
		file >> x >> y >> z;
		vertexB[indice++] = x;
		vertexB[indice++] = y;
		vertexB[indice++] = z;
		i += 3;
	}

	i = indice = 0;
	file >> vertexCount;
	aIndex = (int*)malloc(sizeof(int)*vertexCount);
	cout << vertexCount;
	while (i < vertexCount) {
		file >> x >> y >> z;
		aIndex[indice++] = x;
		aIndex[indice++] = y;
		aIndex[indice++] = z;
		i += 3;
	}
	file.close();

	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size, vertexB, GL_STATIC_DRAW);

	free(vertexB);
}

void Cone::drawVBO() {
	applyTransforms();
	glColor3f(color.x, color.y, color.z);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, aIndex);
	glPopMatrix();
}
/*---------------------------------------------------------------------*/


/*-------------------------------------------------
	Patches de Bézier
--------------------------------------------------*/

vector<Teapot> patchs;
vector<Ponto3D> vertices;

void readTeapot(string path) {
	int i, j, pos, ind, np, nv;
	float ponto[3], n;
	string line, token;

	ifstream ifile;
	ifile.open(path, ios::in);

	if (ifile.is_open()) {
		ifile >> np; getline(ifile, line);

		for (i = 0; i < np && getline(ifile, line); i++) {
			Teapot pa = Teapot::Teapot();
			for (j = 0; j < 16; j++) {
				pos = line.find(",");
				token = line.substr(0, pos);
				ind = atof(token.c_str());
				line.erase(0, pos + 1);

				pa.adicionaIndice(ind);
			}
			patchs.push_back(pa);
		}

		ifile >> nv; getline(ifile, line);

		for (i = 0; i < nv && getline(ifile, line); i++) {
			for (j = 0; j < 3; j++) {
				pos = line.find(",");
				token = line.substr(0, pos);
				n = atof(token.c_str());
				line.erase(0, pos + 1);

				ponto[j] = n;
			}
			vertices.push_back(Ponto3D::Ponto3D(ponto[0], ponto[1], ponto[2]));
		}
		ifile.close();
	}
	else {
		cout << "Ficheiro *.patch nao encontrado!" << endl;
	}
}


Ponto3D bernstein(float t, float *p1, float *p2, float *p3, float *p4) {
	float res[3];

	float it = 1.0 - t;

	float b0 = it*it*it;
	float b1 = 3 * t*it*it;
	float b2 = 3 * t*t*it;
	float b3 = t*t*t;

	res[0] = b0*p1[0] + b1*p2[0] + b2*p3[0] + b3*p4[0];
	res[1] = b0*p1[1] + b1*p2[1] + b2*p3[1] + b3*p4[1];
	res[2] = b0*p1[2] + b1*p2[2] + b2*p3[2] + b3*p4[2];

	return Ponto3D::Ponto3D(res[0], res[1], res[2]);
}


Ponto3D bezier(float u, float v, vector<int> pat) {
	float bz[4][3], res[4][3];
	int i, j = 0, k = 0;

	for (i = 0; i < 16; i++) {
		bz[j][0] = vertices[pat[i]].x;
		bz[j][1] = vertices[pat[i]].y;
		bz[j][2] = vertices[pat[i]].z;

		j++;

		if (j % 4 == 0) {
			Ponto3D p = bernstein(u, bz[0], bz[1], bz[2], bz[3]);
			res[k][0] = p.x;
			res[k][1] = p.y;
			res[k][2] = p.z;

			k++;
			j = 0;
		}
	}
	return bernstein(v, res[0], res[1], res[2], res[3]);
}


void escreveTriangulos(Ponto3D p1, Ponto3D p2, Ponto3D p3, ofstream& file) {
	file << p1.x << " " << p1.y << " " << p1.z << endl;
	file << p2.x << " " << p2.y << " " << p2.z << endl;
	file << p3.x << " " << p3.y << " " << p3.z << endl;
}

//Função que trata de cada linha do patch e converte para triangulos a respectiva linha
void patchBezier(int tess, int ip, ofstream& file) {
	float inc = 1.0 / tess, u, v, u2, v2;

	for (int i = 0; i < tess; i++) {
		for (int j = 0; j < tess; j++) {
			u = i*inc;
			v = j*inc;
			u2 = (i + 1)*inc;
			v2 = (j + 1)*inc;

			Ponto3D p0 = bezier(u, v, patchs[ip].getPatch());
			Ponto3D p1 = bezier(u, v2, patchs[ip].getPatch());
			Ponto3D p2 = bezier(u2, v, patchs[ip].getPatch());
			Ponto3D p3 = bezier(u2, v2, patchs[ip].getPatch());

			escreveTriangulos(p0, p2, p3, file);
			escreveTriangulos(p0, p3, p1, file);
		}
	}
}


//Transforma um ficheiro com todos os patch num ficheiro .3d com os pontos respectivos
void initSupBezier(int tess, string nameFile) {
	ofstream file;
	file.open(nameFile);

	file << "TEAPOT\n";
	int num = patchs.size();

	file << (num*tess*tess * 6 * 3) << endl;

	for (int i = 0; i < num; i++)
		patchBezier(tess, i, file); //Trata de cada patch de Bezier
	file.close();
}

void Teapot::write3DtoFile(string filename)
{

}

void Teapot::read3DfromFile(string filename)
{

}

Teapot::Teapot()
{
}

void Teapot::adicionaIndice(int i) {
	indices.push_back(i);
}

Teapot::~Teapot()
{

}

void Teapot::draw()
{

}

void Teapot::criarVBO(string filename)
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