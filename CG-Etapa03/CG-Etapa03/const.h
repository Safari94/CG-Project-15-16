/*
 *	Autores: Daniel Caldas, Jos� Cortez, Marcelo Gon�alves, Ricardo Silva
 *	Data: 2015.05.04
 *	Vers�o: 3.0v
 *
 */

#ifndef _const_h_included_
#define _const_h_included_

#include "formas.h" // Libraria que cont�m ferramentas que geram e desenham figuras

#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <sstream>
#include <regex>
#include <windows.h>

#include "tinyxml.h"
#include "tinystr.h"

// Mensagens de erro mais frequentes
#define ERROR_NUMBER_ARGS "erro: numero de argumentos invalidos.\n\n"
#define ERROR_INVALID_ARGS "error: argumentos invalidos.\n\n"
#define ERROR_COMMAND_NO_EXISTS "error: o comando invocado nao existe.\n\n"
#define ERROR_FILE_NF "error: ficheiro nao existe.\n\n"
#define ERROR_FORMAT_EXCEPTION "error: formato inesperado.\n\n"
#define MESSAGE_HELP "-Manual-\n[exit]: sair do motor3D\n\n[gerador] opcoes:\n\t[rec largura comprimento fich]\n\t[tri lado fich]\n\t[circ raio nlados fich]\n\t[paralel largura comprimento altura fich]\n\t[esfera raio ncamadas nfatias fich]\n\t[cone altura raio nlados ncamadas fich]\n\n[desenhar fich.xml]: desenhar uma cena (apenas fich extensao .xml)\n\n[solar]: desenhar um modelo do sistema solar animado\n\n[help]: consultar o manual\n\n"

// Nomes dos Formas
#define FORMA_TRIANGULO "TRIANGULO"
#define FORMA_CIRCULO "CIRCULO"
#define FORMA_RECTANGULO "RECTANGULO"
#define FORMA_PARALEL "PARALELEPIPEDO"
#define FORMA_ESFERA "ESFERA"
#define FORMA_CONE "CONE"

// Constantes de transforma��es gem�tricas
#define TRANSLATE "TRASNLATE"
#define ROTATE "ROTATE"
#define SCALE "SCALE"

// Algumas librarias com funcionalidades extra
#pragma comment(lib, "devil.lib")
#pragma comment(lib, "glew32.lib")

// Constantes matem�ticas
#define M_PI 3.14159265358979323846 /*Pi*/
#define M_PI_2 1.57079632679489661923132169163975144 /*Pi/2*/

using namespace std;

#endif