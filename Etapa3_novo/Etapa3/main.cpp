
	/*
	*
	*Autores : Miguel Guimar�es, Miguel Rodrigues, Xavier Rodrigues, Emanuel Fernandes
	*Data : 2016 - 03 - 29
	*
	*/

#include "const.h"
#define _USE_MATH_DEFINES
#include <math.h>

Forma* ff;
Cone* cone;
Plano* p;
Triangulo* t;
Circulo* cir;
Caixa* c;
Esfera* ef;


vector<Forma*> Formas; // Vector de apontadores para armazenamentos dos modelos da figura a desenhar

						   //Variaveis uteis para o controlo da camara
	float px, py, pz;
	float alpha, beta;
	float altura;

	//Variavel a ser usada a como op�ao do menu
	int opcao;
	float x_tela, y_tela; //Variaveis para guardar posi��o da tela em que se carrega no rato
	float raio = 10, cam_h = 0, cam_v = 0.5, camh_x = 0, camh_y = 0;
	int estado_botao = 0;
	float olhar[] = { 0,0,0 };

	// Vari�veis auxiliares para leitura de ficheiros XML e povoamento das estruturas de dados
	vector<TransformsWrapper> transforms_atual;
	vector<Grupo> grupos;
	Ponto3D color;

	// Frames
	int frame = 0;
	int timebase = 0;


	/**Fun��o que gera automaticamente ficheiros com os palentas do modelo est�tico do sistema solar */
		vector<string> sistemaSolarEst()
	{
		string p;
		vector<string> planetas;
		p = "gerador esfera 512 100 100 fundo.3d"; planetas.push_back(p);
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

		return planetas;
	}



	void changeSize(int w, int h) {

		// Prevenir divis�o por 0 quando janela � demasiado pequena
		// (N�o podemos criar uma janela com altura 0)
		if (h == 0)
			h = 1;

		float ratio = w * 1.0 / h;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glViewport(0, 0, w, h);
		gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

		glMatrixMode(GL_MODELVIEW);


	}

	// fun��o de conversao de coordenadas esfericas para cartesianas(vai ser usada na fun�ao aseguir)
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Convers�o de coordenadas esf�ricas para coordenadas cartesianas da posi��o da c�mera
		esfTOcart();

		// Atualizar c�mara
		glLoadIdentity();
		

		//C�mera em modo explorador
		gluLookAt(olhar[0]+raio*sin(cam_h + camh_x)*cos(cam_v + camh_y),olhar[1] +raio*sin(cam_v + camh_y),olhar[2]+ raio*cos(cam_h + camh_x)*cos(cam_v + camh_y),
			olhar[0], olhar[1], olhar[2],
			0.0f, 1.0f, 0.0f);

		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, opcao);

		// Loop para desenhar formas
		for (std::vector<Forma*>::iterator it = Formas.begin(); it != Formas.end(); ++it) {
			(*it)->criarVBO("sol.3d");
			//t = new Triangulo();
			//t->geraTriangulo(100, "sol.3d");
			//t->criarVBO("sol.3d");
			//t->carregarImagem();
		}

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

	/**Fun��o que processa a��es do teclado (movimento da c�mera)*/
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

	/*Fun�ao com op�oes para eventos de escolha do menu*/
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
			if (cam_v + 0.05<M_PI_2)   //Para c�mera n�o virar ao contr�rio
				cam_v += 0.05;
			break;
		case GLUT_KEY_DOWN:
			if (cam_v - 0.05>-M_PI_2)  //Para c�mera n�o virar ao contr�rio
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

	/**Fun��o que constr�i o menu*/
	void menu()
	{
		glutCreateMenu(processMenuEvents);

		// Definir entradas do menu
		glutAddMenuEntry("Ver com cores", 1);
		glutAddMenuEntry("Ver apenas linhas", 2);
		glutAddMenuEntry("Ver apenas pontos", 3);

		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}

	/**Inicializa par�metros relativos � camera*/
	void init_camera_parameters()
	{
		px = 0;
		py = 3.0;
		pz = 6.0;
		alpha = 0.5;
		beta = (M_PI / 3);
		raio = 180;
	}



	/**Fun��o que inicializa os par�metros do glut criando a respetiva janela para desenhar a cena*/
	int prepare_glut()
	{
		init_camera_parameters();
		opcao = GL_LINE;
		altura = 1;

		// Inicializa��o
		char *myargv[1];
		int myargc = 1;
		myargv[0] = "builder3d";
		glutInit(&myargc, myargv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(800, 800);
		glutCreateWindow("Mini Builder3D");

		// Registo de fun��es 
		glutDisplayFunc(renderScene);
		glutIdleFunc(renderScene);
		glutReshapeFunc(changeSize);

		// Intera��es Rato/Teclado
		glutDisplayFunc(renderScene);
		glutReshapeFunc(changeSize);
		glutIdleFunc(renderScene);
		glutKeyboardFunc(teclado1);
		glutMouseFunc(rato);
		glutMotionFunc(mov_rato);
		glutSpecialFunc(specialKeyboard);
		

		// Cria��o do menu
		menu();

		// Settings OpenGL
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		/*
		t = new Triangulo();
		t->geraTriangulo(100, "cone.3d");
		t->criarVBO("cone.3d");
		t->carregarImagem();
*/
		printf("end.\n\n");
		// Entrar no ciclo do GLUT
		glutMainLoop();

		return -1;
	}

	/**Fun��o chamada a quando da leitura de um ficheiro XML em readFromXML
	translacoes e rotacoes s�o cumulativas pelo que as vari�veis globais translacoes e rotacoes representam
	transforma��es comuns (definidas no grupo pai)
	*/
	void gerarForma(string filename)
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

		// Set dos novas valores da forma geom�trica
		s->color = color;
		s->readfromFile(filename);
		s->setTransformacoes(grupos.at(grupos.size() - 1).transfs);

		// Adicionar nova forma
		Formas.push_back(s);

	}
	TiXmlElement * ant;
	string ficheiro_a_ler;
	/**Fun��o que faz parse de um nodo <grupo> do ficheiro XML*/
	void readGrupoFromXML(TiXmlElement* element)
	{
		TiXmlElement* modelo;
		TiXmlElement* points;
		const char* name = element->Value();

		if ((strcmp(name, "translacao") == 0) || (strcmp(name, "escala") == 0)) {
			

			TransformsWrapper tw;

			if (strcmp(name, "translacao") == 0) {
				int time = atoi(element->Attribute("tempo"));
				tw.translacao.tempo = time;
				points = element->FirstChildElement("ponto");
				while (points)
				{
					Ponto3D pp;
					pp.x = atof(points->Attribute("X"));
					pp.y = atof(points->Attribute("Y"));
					pp.z = atof(points->Attribute("Z"));
					tw.translacao.pts.push_back(pp);
					points = points->NextSiblingElement("ponto");
				}
				
				tw.nome = "TRANSLATE";
			}
			else {
				Ponto3D p;
				p.x = atof(element->Attribute("X"));
				p.y = atof(element->Attribute("Y"));
				p.z = atof(element->Attribute("Z"));
				tw.escala = p;
				tw.nome = "SCALE";
			}

			// Atualizar transforma��es atuais
			transforms_atual.push_back(tw);
		}
		else if (strcmp(name, "rotacao") == 0) {
			Rotacao r;// = (Rotacao3D*)malloc(sizeof(Rotacao3D));
			r.ang = atof(element->Attribute("angulo"));
			r.x = atof(element->Attribute("eixoX"));
			r.y = atof(element->Attribute("eixoY"));
			r.z = atof(element->Attribute("eixoZ"));

			TransformsWrapper tw;
			tw.rotacao = r;
			tw.nome = "ROTATE";

			// Atualizar transforma��es atuais
			transforms_atual.push_back(tw);
		}
		else if (strcmp(name, "modelos") == 0) {
			modelo = element->FirstChildElement("modelo");
			while (modelo) {
				string filename = modelo->Attribute("ficheiro");
				ficheiro_a_ler = filename;
				color.x = atof(modelo->Attribute("colorX"));
				color.y = atof(modelo->Attribute("colorY"));
				color.z = atof(modelo->Attribute("colorZ"));

				modelo = modelo->NextSiblingElement("modelo");
			}
		}
		else if (strcmp(name, "grupo") == 0) {
			/*Desmarcar para fazer debug dos grupos do XML*/
			//cout << "ID: " << element->Attribute("prof") << "\n";	
			ant = element;
			TiXmlElement * novo = element->FirstChildElement();
			if (novo != NULL)	readGrupoFromXML(element->FirstChildElement());
		}

		/*Cria��o de novo grupo com transforma��es se:
		- Estamos no �ltimo grupo portanto pr�ximo "Element" � nulo;
		- O pr�ximo elemento XML � um grupo, guardamos os dados referentes ao que acabou (ant).
		*/
		if (element->NextSiblingElement() != NULL && strcmp(element->NextSiblingElement()->Value(), "grupo") == 0) {
			Grupo g;

			// Id de profundidade do grupo
			int prof = atoi(ant->Attribute("prof"));
			vector<TransformsWrapper> acumulador;

			// Calcular indice do irm�o subtrair 1 e obtemos o(s) pai(s)
			int indice_do_pai = -1;
			if (grupos.size() > 0) {
				int i;
				for (i = grupos.size() - 1; prof <= grupos.at(i).id; i--); // Para no irmao vari�vel i

				if (i <= 0) {
					indice_do_pai = 0;
				}
				else {
					indice_do_pai = i;
				}
			}

			// Herdar transforma��es at� ao indice_do_pai (se indice_do_pai==-1 n�o herda nada)
			if (indice_do_pai != -1) {
				//for (int k = 0; k <= indice_do_pai; k++){
				// Percorrer e copiar transforma��es de um dado grupo pai
				for (int i = 0; i < grupos.at(indice_do_pai).transfs.size(); i++) {
					acumulador.push_back(grupos.at(indice_do_pai).transfs.at(i));
				}
				//}
			}
			// Fazer acumula��es se existirem
			if (acumulador.size()>0) {
				g = Grupo(acumulador);
				g.id = prof;

				// Agora podemos atribuir as transforma��es do PR�PRIO grupo
				for (int i = 0; i < transforms_atual.size(); i++) {
					g.transfs.push_back(transforms_atual.at(i));
				}
			}
			else {
				// N�o existem transforma��es para acumular (e.g. In�cio do ficheiro ou grupo sem pai(s))
				g = Grupo(transforms_atual);
				g.id = prof;
			}

			// Incluir o novo grupo no vetor de grupos
			grupos.push_back(g);

			// Eliminar todas as transforma��es ATUAIS (renicializar o vetor para um novo grupo)
			transforms_atual = vector <TransformsWrapper>();

			// CRIAR UM MODELO:
			/*1 - Criar modelo*/
			/*1.1 - Associar respetivo array de transforma��es*/
			gerarForma(ficheiro_a_ler);
		}

		// Chamada recursiva
		if (element->NextSiblingElement() != NULL) {
			readGrupoFromXML(element->NextSiblingElement());
		}
	}

	/**Parser xml extrai nomes de ficheiros que conteem os tri�ngulos das figuras a desenhar*/
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

		readGrupoFromXML(element);
	}

	/**Fun��o que testa se um dado ficheiro possu� extens�o .xml*/
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
	

	/**Fun��o que prepara as figuras para desenhar a partir de uma fonte .xml e invoca o ciclo glut*/
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
				readFromXML(filename); // Internamente � leitura geramos os modelos enquanto que na 1� etapa
									   // eram gerados nesta mesma fun��o

									   // Chamada expl�cita para desenhar cena
				prepare_glut();
			}
		}
		catch (invalid_argument ia) {
			cout << ERROR_INVALID_ARGS;
		}
		return -1;
	}

	/** Fun��o que remove os espa�os em branco de uma string*/
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

				// Gerar ficheiro com os tri�ngulos
				cout << "a gerar plano ...\n";
				Plano* p = new Plano();
				p->geraPlano(l, c, file);
				cout << "end.\n\n";
			}
			else if ((args.at(1).compare("circ") == 0) && args.size() == 5) {
				file = args.at(4);
				float raio1 = stof(args.at(2));
				float nlados = stof(args.at(3));

				// Gerar ficheiro com os tri�ngulos
				cout << "a gerar circulo ...\n";
				Circulo* c = new Circulo();
				c->geraCirculo(raio1, nlados, file);
				cout << "end.\n\n";
			}
			else if ((args.at(1).compare("tri") == 0) && args.size() == 4) {
				file = args.at(3);
				float lado = stof(args.at(2));

				// Gerar ficheiro com os tri�ngulos
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

				// Gerar ficheiro com os tri�ngulos
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

				// Gerar ficheiro com os tri�ngulos
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

				// Gerar ficheiro com os tri�ngulos
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

	/**Func�o interpretar*/
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
			// Como o comando invocado n�o � nenhum dos anteriores, devolve o erro correspondente
			cout << ERROR_COMMAND_NO_EXISTS;
			return -1;
		}
	}

	/**Fun��o interpretador*/
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

	/**Fun��o main*/
	int main() {
		interpretador();
		return 0;
	}



