#ifndef	_GAME_H_
#define	_GAME_H_

#include <windows.h>

#include "settings.h"
#include "colors.h"
#include "util.h"

// Constantes relacionadas ao quadro
#define BOARD_CHAR_TOP_LEFT 201
#define BOARD_CHAR_TOP_RIGHT 187
#define BOARD_CHAR_BOTTOM_LEFT 204
#define BOARD_CHAR_BOTTOM_RIGHT 188
#define BOARD_CHAR_TOP_BOTTOM 205
#define BOARD_CHAR_SIDE 186
#define BOARD_CHAR_CORNER_TOP 203
#define BOARD_CHAR_CORNER_BOTTOM 206
#define BOARD_CHAR_CORNER_LEFT 200
#define BOARD_CHAR_CORNER_RIGHT 188

#define BOARD_SIZE_X ( GAME_PIECES * 2 + 1 ) + ( GAME_PIECES + 2 ) + 3
#define BOARD_SIZE_Y GAME_ATTEMPTS + 2

// Declara��o das cores usadas nas pe�as
#define GAME_COLORS 4 // Para adicionar uma cor � preciso declara-la e adiciona-la � array PIECE_COLORS no ficheiro game.c
#define PIECE_FOREGROUND_GREEN FOREGROUND_GREEN_INTENSE
#define PIECE_FOREGROUND_CYAN FOREGROUND_CYAN_INTENSE
#define PIECE_FOREGROUND_MAGENTA FOREGROUND_MAGENTA_INTENSE
#define PIECE_FOREGROUND_YELLOW FOREGROUND_YELLOW_INTENSE

// Declara��o da vari�vel global para as cores usadas nas pe�as
extern const WORD PIECE_COLORS [ GAME_COLORS ];

// Declara��o da vari�vel global para o som do jogo
extern int gameSound;

// Declara��o da array global para a estrutura do quadro
extern char boardArray [ BOARD_SIZE_Y + 2 ] [ BOARD_SIZE_X + 1 ];

// Declara��o da array global para as pe�as do jogo
extern int gameSecretKey [ GAME_PIECES ];

// Declara��o dos prot�tipos das fun��es
void prepareGame ( HANDLE, ScreenResolution ); // Prepara o jogo, chama a fun��o para criar o quadro, gerar a chave secreta e iniciar o jogo
void drawBoard ( HANDLE, ScreenResolution, int, int ); // Desenha o quadro do jogo no ecr�
void prepareBoardArray ( int, int ); // Prepara a Array do quadro do jogo
void prepareGameKey ( HANDLE, ScreenResolution, CONSOLE_SCREEN_BUFFER_INFO, int, int, int ); // Prepara a chave secreta
void generateGameKey ( void ); // Gera a chave secreta
void startGame ( HANDLE, ScreenResolution, CONSOLE_SCREEN_BUFFER_INFO ); // Inicia o algoritmo do jogo
void displayWinScreen ( HANDLE, ScreenResolution, int ); // Apresenta o ecr� de vitoria no caso de acerta na combina��o antes de terminar as tentativas
void displayLossScreen ( HANDLE, ScreenResolution ); // Apresenta o ecr� de perdador no caso de n�o encontrar a solu��o no limite de tentativas

#endif	/* Not _GAME_H_ */
