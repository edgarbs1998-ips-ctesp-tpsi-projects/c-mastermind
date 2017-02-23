#ifndef	_DEFINES_H_
#define	_DEFINES_H_

#include "colors.h"

// Defina como 0 para vers�es de lan�amento
#define DEBUG 0

// Constantes gerais
#define APP_LANGUAGE "Portuguese"
#define GAME_TITLE "Mastermind"

// Constantes relacionadas ao jogo
#define GAME_ATTEMPTS 10 // Sistema din�mico, possibilita alterar o n�mero de tentativas
#define GAME_PIECES 4 // Sistema din�mico, possibilita alterar o n�mero de pe�as da chave

// Constantes relacionadas ao quadro do jogo
#define BOARD_COLOR BACKGROUND_CYAN // Cor de fundo do quadro do jogo

// Constantes relacionadas ao ranking
#define FILE_RANKING_PATH "ranking.txt" // Localiza��o do ficheiro TXT do ranking
#define RANK_TOP_SIZE 10 // Quantidade de registos guardados no ranking

#define MAX_PLAYER_NAME 1 + 20 // Tamanho m�ximo para o nome do jogador
#define MAX_PLAYER_NAME_STRING "20" // Manter igual a MAX_PLAYER_NAME

#endif	/* Not _DEFINES_H_ */
