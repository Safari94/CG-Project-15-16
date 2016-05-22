/*
*
*Autores : Miguel Guimarães, Miguel Rodrigues, Xavier Rodrigues, Emanuel Fernandes
*Data : 2016 - 03 - 29
*
*/

#include "const.h"
#include "tinyxml.h"
#include "tinystr.h"
#define _USE_MATH_DEFINES
#include <math.h>

Forma* ff;
Cone* cone;
Plano* p;
Triangulo* t;
Circulo* cir;
Caixa* c;
Esfera* ef;

vector<string> files = vector<string>();

vector<Forma*> Formas; // Vector de apontadores para armazenamentos dos modelos da figura a desenhar

					   //Variaveis uteis para o controlo da camara
float px, py, pz;
float alpha, beta;
float altura;

//Variavel a ser usada a como opçao do menu
int opcao;
float x_tela, y_tela; //Variaveis para guardar posição da tela em que se carrega no rato
float raio = 10, cam_h = 0, cam_v = 0.5, camh_x = 0, camh_y = 0;
int estado_botao = 0;
float olhar[] = { 0,0,0 };

// Variáveis auxiliares para leitura de ficheiros XML e povoamento das estruturas de dados
vector<TransformsWrapper> transforms_atual;
vector<Grupo> grupos;
Ponto3D color;

// Frames
int frame = 0;
int timebase = 0;


// Luzes da cena a desenhar
vector<Luz> luzes;
int luzes_counter;

// Variáveis globais auxiliares de leitura de ficheiros XML
vector<string> texturas;
vector<float> diffColors;
vector<int> mode;
vector<string> ficheiros_a_ler;
TiXmlElement * ant;
string ficheiro_a_ler;


/**Função que gera automaticamente ficheiros com os palentas do modelo estático do sistema solar */
vector<string> sistemaSolarEst()
{
	string p;
	vector<string> planetas;
	p = "gerador esfera 512 30 30 fundo.3d"; planetas.push_back(p);
	p = "gerador esfera 17 30 30 sol.3d"; planetas.push_back(p);
	p = "gerador esfera 1.5 30 30 mercurio.3d"; planetas.push_back(p);
	p = "gerador esfera 1.9 30 30 venus.3d"; planetas.push_back(p);
	p = "gerador esfera 2 30 30 terra.3d"; planetas.push_back(p);
	p = "gerador esfera 0.5 30 30 lua.3d"; planetas.push_back(p);
	p = "gerador esfera 1.6 30 30 marte.3d"; planetas.push_back(p);
	p = "gerador esfera 9 30 30 jupiter.3d"; planetas.push_back(p);
	p = "gerador esfera 8 30 30 saturno.3d"; planetas.push_back(p);
	p = "gerador esfera 4.7 30 30 urano.3d"; planetas.push_back(p);
	p = "gerador esfera 4.5 30 30 neptuno.3d"; planetas.push_back(p);
	//readTeapot("teapot.patch");
	//initSupBezier(10, "teapot.3d");

	return planetas;
}
	/**Função chamada a quando da leitura de um ficheiro XML em readFromXML
	translacoes e rotacoes são cumulativas pelo que as variáveis globais translacoes e rotacoes representam
	transformações comuns (definidas no grupo pai)
	*/
void gerarForma(string filename, int i)
{
	ifstream f(filename);
	string form;
	f >> form;

	Forma* s = NULL;
	if (form.compare(FORMA_PLANO) == 0) {
		s = new Plano();
	}
	else if (form.compare(FORMA_TRIANGULO) == 0) {
		s = new Triangulo();
	}
	else if (form.compare(FORMA_CIRCULO) == 0) {
		s = new Circulo();
	}
	else if (form.compare(FORMA_CAIXA) == 0) {
		s = new Caixa();
	}
	else if (form.compare(FORMA_ESFERA) == 0) {
		s = new Esfera();
	}
	else if (form.compare(FORMA_CONE) == 0) {
		s = new Cone();
	}
	else if (form.compare("TEAPOT") == 0) {
		s = new Teapot();
	}

	if (mode[i] == 0){
		// Forma tem uma textura associada
		s->textfich = texturas[i];
	}
	else if (mode[i] == 1){
		// Forma tem cor difusa
		s->color.x = diffColors[i];
		s->color.y = diffColors[i + 1];
		s->color.z = diffColors[i + 2];
		s->ficheiro = "EMPTY"; // Quando uma forma não tem textura nesse campo está escrito EMPTY
	}

	//s->read3DfromFile(filename); // VERSÃO 2 e anteriores

	s->setTransformacoes(grupos.at(grupos.size() - 1).transfs);
	files.push_back(filename);
	Formas.push_back(s);
	s->printAllTransforms();

}

void readLights(TiXmlElement* element)
{
	TiXmlElement* modelo;

	Luz luz;
	modelo = element->FirstChildElement("luz");

	while (modelo != NULL){
		float *values;
		string tipo = modelo->Attribute("tipo");
		int gl_i;

		if (tipo.compare("POINT") == 0){
			gl_i = atoi(modelo->Attribute("id"));
			values = (float*)malloc(4 * sizeof(float));
			values[0] = atof(modelo->Attribute("posX"));
			values[1] = atof(modelo->Attribute("posY"));
			values[2] = atof(modelo->Attribute("posZ"));
			values[3] = atof(modelo->Attribute("pv"));
		}
		else if (tipo.compare("AMBIENT") == 0){
			gl_i = atoi(modelo->Attribute("id"));
			values = (float*)malloc(3 * sizeof(float));
			values[0] = atof(modelo->Attribute("ambR"));
			values[1] = atof(modelo->Attribute("ambG"));
			values[2] = atof(modelo->Attribute("ambB"));
		}
		else if (tipo.compare("DIFFUSE") == 0){
			gl_i = atoi(modelo->Attribute("id"));
			values = (float*)malloc(3 * sizeof(float));
			values[0] = atof(modelo->Attribute("diffR"));
			values[1] = atof(modelo->Attribute("diffG"));
			values[2] = atof(modelo->Attribute("diffB"));
		}

		luz.gl_i = gl_i;
		luz.tipo = tipo;
		luz.values = values;

		luzes.push_back(luz);

		luzes_counter++;

		modelo = modelo->NextSiblingElement("luz");
	}
}


/**Função que faz parse de um nodo <grupo> do ficheiro XML*/
void readGrupoFromXML(TiXmlElement* element)
{
	TiXmlElement* modelo;

	ficheiros_a_ler = vector<string>();
	mode = vector<int>();

	const char* name = element->Value();

	if (strcmp(name, "escala") == 0){
		Ponto3D p;
		p.x = atof(element->Attribute("X"));
		p.y = atof(element->Attribute("Y"));
		p.z = atof(element->Attribute("Z"));

		TransformsWrapper tw;
		tw.nome = "SCALE";
		tw.escala = p;

		// Atualizar transformações atuais
		transforms_atual.push_back(tw);
	}
	else if (strcmp(name, "translacao") == 0){
		TransformsWrapper tw;
		tw.nome = "TRANSLATE";

		tw.translacao.b = 0;
		tw.translacao.tempo = atoi(element->Attribute("tempo")); // Tempo da translação
		tw.translacao.first.x = tw.translacao.first.y = tw.translacao.first.z = 0;

		TiXmlElement * pontos;
		TiXmlElement * aux;
		pontos = element;

		vector<Ponto3D> vp = vector<Ponto3D>();
		aux = pontos->FirstChildElement("ponto");
		while (aux){
			Ponto3D paux;
			paux.x = atof(aux->Attribute("X"));
			paux.y = atof(aux->Attribute("Y"));
			paux.z = atof(aux->Attribute("Z"));
			vp.push_back(paux);

			aux = aux->NextSiblingElement("ponto");
		}

		tw.translacao.pontos = vp;
		transforms_atual.push_back(tw);
	}
	else if (strcmp(name, "rotacao") == 0){
		Rotacao3D r;
		r.tempo = atoi(element->Attribute("tempo"));
		r.ang = atof(element->Attribute("angulo"));
		r.x = atof(element->Attribute("eixoX"));
		r.y = atof(element->Attribute("eixoY"));
		r.z = atof(element->Attribute("eixoZ"));

		TransformsWrapper tw;
		tw.rotacao = r;
		tw.nome = "ROTATE";

		// Atualizar transformações atuais
		transforms_atual.push_back(tw);
	}
	else if (strcmp(name, "modelos") == 0){
		modelo = element->FirstChildElement("modelo");
		while (modelo){
			string filename = modelo->Attribute("ficheiro");
			ficheiro_a_ler = filename;

			// Textura ou cor difusa para um dado modelo
			if (modelo->Attribute("textura") != NULL){
				texturas.push_back(modelo->Attribute("textura"));
				diffColors.push_back(0.0);
				diffColors.push_back(0.0);
				diffColors.push_back(0.0);
				mode.push_back(0);
			}
			else if (modelo->Attribute("diffR") != NULL){
				diffColors.push_back(atof(modelo->Attribute("diffR")));
				diffColors.push_back(atof(modelo->Attribute("diffG")));
				diffColors.push_back(atof(modelo->Attribute("diffB")));

				texturas.push_back("empty");
				mode.push_back(1);
			}

			ficheiros_a_ler.push_back(ficheiro_a_ler);

			modelo = modelo->NextSiblingElement("modelo");
		}
	}
	else if (strcmp(name, "grupo") == 0){
		/*Desmarcar para fazer debug dos grupos do XML*/
		//cout << "ID: " << element->Attribute("prof") << "\n";	
		ant = element;
		TiXmlElement * novo = element->FirstChildElement();
		if (novo != NULL)	readGrupoFromXML(element->FirstChildElement());
	}

	/*Criação de novo grupo com transformações se:
	- Estamos no último grupo portanto próximo "Element" é nulo;
	- O próximo elemento XML é um grupo, guardamos os dados referentes ao que acabou (ant).
	*/
	if (element->NextSiblingElement() != NULL && strcmp(element->NextSiblingElement()->Value(), "grupo") == 0){
		Grupo g;

		// Id de profundidade do grupo
		int prof = atoi(ant->Attribute("prof"));
		vector<TransformsWrapper> acumulador;

		// Calcular indice do irmão subtrair 1 e obtemos o(s) pai(s)
		int indice_do_pai = -1;
		if (grupos.size() > 0){
			int i;
			for (i = grupos.size() - 1; i >= 0 && prof <= grupos.at(i).id; i--); // Para no irmao variável i

			if (i < 0){
				indice_do_pai = -1;
			}
			else{
				indice_do_pai = i;
			}
		}

		// Herdar transformações até ao indice_do_pai (se indice_do_pai==-1 não herda nada)
		if (indice_do_pai != -1){
			// Percorrer e copiar transformações de um dado grupo pai
			for (int i = 0; i < grupos.at(indice_do_pai).transfs.size(); i++){
				acumulador.push_back(grupos.at(indice_do_pai).transfs.at(i));
			}
		}
		// Fazer acumulações se existirem
		if (acumulador.size()>0){
			g = Grupo(acumulador);
			g.id = prof;

			// Agora podemos atribuir as transformações do PRÓPRIO grupo
			for (int i = 0; i < transforms_atual.size(); i++){
				g.transfs.push_back(transforms_atual.at(i));
			}
		}
		else{
			// Não existem transformações para acumular (e.g. Início do ficheiro ou grupo sem pai(s))
			g = Grupo(transforms_atual);
			g.id = prof;
		}

		// Incluir o novo grupo no vetor de grupos
		grupos.push_back(g);

		// Eliminar todas as transformações ATUAIS (renicializar o vetor para um novo grupo)
		transforms_atual = vector <TransformsWrapper>();

		// CRIAR UM MODELO:
		/*1 - Criar modelo*/
		/*1.1 - Associar respetivo array de transformações*/
		int n = ficheiros_a_ler.size();
		int i;
		for (i = 0; i < n; i++){
			gerarForma(ficheiros_a_ler[i], i);
		}
		ficheiros_a_ler = vector<string>();
		mode = vector<int>();
		texturas = vector<string>();
		diffColors = vector<float>();
	}

	// Chamada recursiva
	if (element->NextSiblingElement() != NULL){
		readGrupoFromXML(element->NextSiblingElement());
	}
}
}

/**Parser xml extrai nomes de ficheiros que conteem os triângulos das figuras a desenhar*/
void readFromXML(string filename)
{
	const char * nomeFich = filename.c_str();

	TiXmlDocument doc(nomeFich);
	doc.LoadFile();
	TiXmlHandle docHandle(&doc);

	TiXmlNode * node = docHandle.FirstChild("imagem").ToNode();

	TiXmlElement * element;
	TiXmlElement * aux;

	element = node->FirstChildElement();

	luzes = vector<Luz>();
	luzes_counter = 0;
	mode = vector<int>();
	texturas = vector<string>();

	readLights(element);
	element = node->FirstChildElement();

	readGrupoFromXML(element);
}

/**Função que testa se um dado ficheiro possuí extensão .xml*/
bool isXML(string filename)
{
	if (regex_match(filename, regex(".+\.xml"))) return true;
	else {
		cout << ERROR_FORMAT_EXCEPTION;
		return false;
	}
}

/**Verifica se um dado ficheiro existe na diretoria do projeto*/
bool lookUpFile(string filename)
{
	ifstream file(filename);
	if (file) {
		return true;
	}
	else {
		cout << ERROR_FILE_NF;
		return false;
	}
}



void changeSize(int w, int h) {

	// Prevenir divisão por 0 quando janela é demasiado pequena
	// (Não podemos criar uma janela com altura 0)
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);


}

// função de conversao de coordenadas esfericas para cartesianas(vai ser usada na funçao aseguir)
void esfTOcart()
{
	pz = raio * cosf(beta) * cosf(alpha);
	px = raio * cosf(beta) * sinf(alpha);
	py = raio * sinf(beta);
}

/**Desenha a cena (esqueleto openGL)*/
void renderScene(void)
{
	float fps;
	int time;
	char s[64];
	// Limpar buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Conversão de coordenadas esféricas para coordenadas cartesianas da posição da câmera
	esfTOcart();

	// Atualizar câmara
	glLoadIdentity();
	

	//Câmera em modo explorador
	gluLookAt(olhar[0]+raio*sin(cam_h + camh_x)*cos(cam_v + camh_y),olhar[1] +raio*sin(cam_v + camh_y),olhar[2]+ raio*cos(cam_h + camh_x)*cos(cam_v + camh_y),
		olhar[0], olhar[1], olhar[2],
		0.0f, 1.0f, 0.0f);

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, opcao);

	for (Luz luz : luzes){
		if (luz.tipo.compare("POINT") == 0){
			glLightfv(GL_LIGHT0 + luz.gl_i, GL_POSITION, luz.values);
		}
		else if (luz.tipo.compare("AMBIENT") == 0){
			glLightfv(GL_LIGHT0 + luz.gl_i, GL_AMBIENT, luz.values);
		}
		else if (luz.tipo.compare("DIFFUSE") == 0){
			glLightfv(GL_LIGHT0 + luz.gl_i, GL_DIFFUSE, luz.values);
		}
	}
	float white[4] = { 1, 1, 1, 1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);


	// Loop para desenhar formas
	for (std::vector<Forma*>::iterator it = Formas.begin(); it != Formas.end(); ++it) {
		(*it)->desenharVBO();
	
		/*
		t = new Triangulo();
		t->geraTriangulo(100, "sol.3d");
		t->criarVBO("sol.3d");
		t->carregarImagem();*/
	}
	//ef->desenharVBO();
	//ef->desenharVBO();

	//  janela com as frames
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame*1000.0 / (time - timebase);
		timebase = time;
		frame = 0;
		sprintf_s(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
}

	glutSwapBuffers();
}

/**Função que processa ações do teclado (movimento da câmera)*/
void teclado1(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		olhar[0] -= 5; // Move left
		break;
	case 'd':
		olhar[0] += 5; // Move right
		break;
	case 'w':
		olhar[1] += 5; // move up
		break;
	case 'x':
		 olhar[1] -= 5; // move down
		break;
	case'e': // Menos zoom
		if (raio>0) raio -= 5;
		break;
	case 'r': // Mais zoom
		raio += 5;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void rato(int botao, int estado, int x, int y) {
	if (botao == GLUT_LEFT_BUTTON) {
		if (estado == GLUT_DOWN) {
			estado_botao = 1;
			x_tela = x;
			y_tela = y;
		}
		else {
			estado_botao = 0;
			cam_v += camh_y;
			cam_h += camh_x;
			camh_x = 0;
			camh_y = 0;
		}
	}
}

void mov_rato(int x, int y) {
	float teste;
	if (estado_botao) {
		if (x_tela != x)
			camh_x = (x_tela - x)*0.007;

		if (y_tela != y) {
			teste = (y_tela - y)*0.007;
			if (teste + cam_v>-M_PI_2 && teste + cam_v<M_PI_2)
				camh_y = teste;
		}

		glutPostRedisplay();
	}
}

/*Funçao com opçoes para eventos de escolha do menu*/
void processMenuEvents(int op)
{
	switch (op) {
	case 1:
		opcao = GL_FILL; // tudo preenchido
		break;
	case 2:
		opcao = GL_LINE; // so as linhas
		break;
	case 3:
		opcao = GL_POINT; // so com pontos
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void specialKeyboard(int op, int x, int y) {
	switch (op) {
	case GLUT_KEY_UP:
		if (cam_v + 0.05<M_PI_2)   //Para câmera não virar ao contrário
			cam_v += 0.05;
		break;
	case GLUT_KEY_DOWN:
		if (cam_v - 0.05>-M_PI_2)  //Para câmera não virar ao contrário
			cam_v -= 0.05;
		break;

	case GLUT_KEY_LEFT:
		cam_h -= 0.05;
		break;
	case GLUT_KEY_RIGHT:
		cam_h += 0.05;
		break;

	default:
		break;

	}
	glutPostRedisplay();
}

/**Função que constrói o menu*/
void menu()
{
	glutCreateMenu(processMenuEvents);

	// Definir entradas do menu
	glutAddMenuEntry("Ver com cores", 1);
	glutAddMenuEntry("Ver apenas linhas", 2);
	glutAddMenuEntry("Ver apenas pontos", 3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/**Inicializa parâmetros relativos à camera*/
void init_camera_parameters()
{
	px = 0;
	py = 3.0;
	pz = 6.0;
	alpha = 0.5;
	beta = (M_PI / 3);
	raio = 180;
}



/**Função que inicializa os parâmetros do glut criando a respetiva janela para desenhar a cena*/
int prepare_glut(string filename)
{
	init_camera_parameters();
	opcao = GL_LINE;
	altura = 1;

	// Inicialização
	char *myargv[1];
	int myargc = 1;
	glewExperimental = GL_TRUE;
	myargv[0] = "builder3d";
	glutInit(&myargc, myargv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Mini Builder3D");

	// Registo de funções 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Interações Rato/Teclado
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(teclado1);
	glutMouseFunc(rato);
	glutMotionFunc(mov_rato);
	glutSpecialFunc(specialKeyboard);
	

	// Criação do menu
	menu();

	// Settings OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glewInit();
	ilInit();


	//Activar Buffers
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
*/
	//t = new Triangulo();
	//t->geraTriangulo(100, "esfera.3d");
	//ef = new Esfera();
	//ef->geraEsfera(17, 50, 50, "esfera2.3d");
	//ef->criarVBO("esfera.3d");
	/*t->carregarImagem();
*/		

	glEnable(GL_LIGHTING);
	// Ciclo para inicialização das luzes individuais
	for (int i = 0; i < luzes.size(); i++){
		glEnable(GL_LIGHT0 + i);
	}

	cout << "a criar VBOs e a carregar imagens ...\n";

	int n = Formas.size();
	for (int i = 0; i < n; i++) {
		Formas[i]->criarVBO(files[i]);
		Formas[i]->carregarImagem();
	}
	iniciar(n);
	printf("end.\n\n");
	// Entrar no ciclo do GLUT
	glutMainLoop();

	return -1;
}




/**Função que prepara as figuras para desenhar a partir de uma fonte .xml e invoca o ciclo glut*/
int gerar_figura(vector<string> args)
{
	string filename;
	string form;

	if (args.size() < 2) {
		cout << ERROR_NUMBER_ARGS;
		return -1;
	}
	try {
		filename = args.at(1);
		if (isXML(filename) && lookUpFile(filename)) {
			printf("a carregar dados para gerar figura ...\n");
			readFromXML(filename);
			prepare_glut(filename);
			 // Internamente à leitura geramos os modelos enquanto que na 1ª etapa
								   // eram gerados nesta mesma função

								   // Chamada explícita para desenhar cena
			
		}
	}
	catch (invalid_argument ia) {
		cout << ERROR_INVALID_ARGS;
	}
	return -1;
}

/** Função que remove os espaços em branco de uma string*/
string removeSpaces(string input) {
	input.erase(remove(input.begin(), input.end(), ' '), input.end());
	return input;
}

/**Comando gerador*/
int gerador(vector<string> args)
{
	string file;

	if (args.size() < 3) {
		cout << ERROR_NUMBER_ARGS;
		return -1;
	}
	try {
		if ((args.at(1).compare("pln") == 0) && args.size() == 5) {
			file = args.at(4);
			float c = stof(args.at(2));
			float l = stof(args.at(3));

			// Gerar ficheiro com os triângulos
			cout << "a gerar plano ...\n";
			Plano* p = new Plano();
			p->geraPlano(l, c, file);
			cout << "end.\n\n";
		}
		else if ((args.at(1).compare("circ") == 0) && args.size() == 5) {
			file = args.at(4);
			float raio1 = stof(args.at(2));
			float nlados = stof(args.at(3));

			// Gerar ficheiro com os triângulos
			cout << "a gerar circulo ...\n";
			Circulo* c = new Circulo();
			c->geraCirculo(raio1, nlados, file);
			cout << "end.\n\n";
		}
		else if ((args.at(1).compare("tri") == 0) && args.size() == 4) {
			file = args.at(3);
			float lado = stof(args.at(2));

			// Gerar ficheiro com os triângulos
			cout << "a gerar triangulo ...\n";
			Triangulo* p = new Triangulo();
			p->geraTriangulo(lado, file);
			cout << "end.\n\n";
		}
		else if ((args.at(1).compare("caixa") == 0) && args.size() == 6) {
			file = args.at(5);
			float l = stof(args.at(2));
			float c = stof(args.at(3));
			float h = stof(args.at(4));

			// Gerar ficheiro com os triângulos
			cout << "a gerar Caixa ...\n";
			Caixa* p = new Caixa();
			p->geraCaixa(l, c, h, file);
			cout << "end.\n\n";
		}
		else if ((args.at(1).compare("esfera") == 0) && args.size() == 6) {
			file = args.at(5);
			float raio1 = stof(args.at(2));
			int fatias = stof(args.at(3));
			int camadas = stoi(args.at(4));

			// Gerar ficheiro com os triângulos
			cout << "a gerar esfera ...\n";
			Esfera* esfera = new Esfera();
			esfera->geraEsfera(raio1, fatias, camadas, file);
			cout << "end.\n\n";
		}
		else if (args.at(1).compare("esferaVBO") == 0) {
			cout << "a gerar esfera ...\n";
			Esfera* esf = new Esfera();		
			esf->criarVBO("sol.3d");
			cout << "end.\n\n";
		}
		else if ((args.at(1).compare("cone") == 0) && args.size() == 7) {
			file = args.at(6);
			float h = stof(args.at(2));
			float r = stof(args.at(3));
			float nlados = stof(args.at(4));
			float ncamadas = stof(args.at(5));

			// Gerar ficheiro com os triângulos
			cout << "a gerar cone ...\n";
			Cone* c = new Cone();
			c->geraCone(h, r, nlados, ncamadas, file);
			cout << "end.\n\n";
		}
		else { cout << ERROR_INVALID_ARGS; }
	}
	catch (invalid_argument ia) {
		cout << ERROR_INVALID_ARGS;
	}
	return -1;
}

/**Funcão interpretar*/
int interpretar(string linha) {
	istringstream ss(linha);
	string token;
	int cont = 0;
	vector<string> playerInfoVector;

	while (std::getline(ss, token, ' ')) {
		token = removeSpaces(token);
		if (token.compare("") != 0) {
			playerInfoVector.push_back(token);
			cont++;
		}
	}

	// Comando gerar
	if (playerInfoVector.at(0).compare("gerador") == 0 && cont != 0) {
		return gerador(playerInfoVector);
	}
	else if (playerInfoVector.at(0).compare("desenhar") == 0 && cont != 0) {
		return gerar_figura(playerInfoVector);
	}
	else if (playerInfoVector.at(0).compare("solar") == 0 && cont != 0) {
		vector<string> planetas = sistemaSolarEst();
		for (int i = 0; i < planetas.size(); i++) interpretar(planetas.at(i));
		playerInfoVector.push_back("solar.xml");
		return gerar_figura(playerInfoVector);
	}
	else if (playerInfoVector.at(0).compare("help") == 0 && cont != 0) {
		cout << MESSAGE_HELP;
	}
	else if (playerInfoVector.at(0).compare("exit") == 0 && cont != 0) {
		return 0;
	}
	else if (playerInfoVector.at(0).compare("gerar_sistema") == 0 && cont != 0) {
		vector<string> planetas;
		planetas = sistemaSolarEst();

		for (auto &planeta : planetas) {
			string buf; // Have a buffer string
			stringstream ss(planeta); // Insert the string into a stream
			vector<string> tokens; // Create vector to hold our words
			while (ss >> buf)
				tokens.push_back(buf);
			gerador(tokens);
		}

		cout << MESSAGE_GENERATE_SS;
	}
	else {
		// Como o comando invocado não é nenhum dos anteriores, devolve o erro correspondente
		cout << ERROR_COMMAND_NO_EXISTS;
		return -1;
	}
}

/**Função interpretador*/
void interpretador()
{
	int resultado = 0;
	string input = "";
	int ciclo = 1;

	for (printf("$builder3D > "); ciclo && getline(cin, input); printf("$builder3D > ")) {
		if (input.compare("") != 0) {
			resultado = interpretar(input);
			if (resultado == 0)
				ciclo = 0;
		}
	}
}

/**Função main*/
int main() {
	interpretador();
	return 0;
}



