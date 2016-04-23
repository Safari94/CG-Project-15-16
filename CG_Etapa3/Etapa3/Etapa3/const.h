/*
*
*Autores : Miguel Guimarães, Miguel Rodrigues, Xavier Rodrigues, Emanuel Fernandes
*Data : 2016 - 03 - 01
*
*/

#ifndef _const_h_included_
#define _const_h_included_

#include "forma.h" // Libraria que contêm ferramentas que geram e desenham figuras

#include <iostream>
#include <fstream>
#include <glut.h>
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
#define MESSAGE_HELP "-Manual-\n[exit]: sair do builder3D\n\n[gerador] opcoes:\n\t[pln largura comprimento fich]\n\t[tri lado fich]\n\t[circ raio nlados fich]\n\t[caixa largura comprimento altura fich]\n\t[esfera raio ncamadas nfatias fich]\n\t[cone altura raio nlados ncamadas fich]\n\n[desenhar fich.xml]: desenhar a figura (apenas fich extensao .xml)\n\n[help]: consultar o manual\n\n"
#define MESSAGE_GENERATE_SS "Esferas.3d para o modelo do sistema solar criadas"

// Nomes dos Formas
#define FORMA_TRIANGULO "TRIANGULO"
#define FORMA_CIRCULO "CIRCULO"
#define FORMA_PLANO "PLANO"
#define FORMA_CAIXA "CAIXA"
#define FORMA_ESFERA "ESFERA"
#define FORMA_CONE "CONE"

// Constantes matemáticas
# define M_PI 3.14159265358979323846 /*Pi*/

using namespace std;

#endif