#ifndef	_RANKING_H_
#define	_RANKING_H_

#include <windows.h>
#include <time.h>

#include "settings.h"
#include "util.h"

// Declaração da estrutura PlayerScore para guardar o registo dos jogos
typedef struct {
    char name [ MAX_PLAYER_NAME ];
    int score;
    struct tm time;
} PlayerScore;

// Declaração dos protótipos das funções
void displayRanking ( HANDLE, ScreenResolution ); // Mostra as melhores jogadas guardadas no ficheiro
void emptyRanking ( HANDLE, ScreenResolution ); // Mostra uma mensagem de informativa no caso de não existirem jogadas para apresentar
void confirmResetRanking ( HANDLE, ScreenResolution ); // Confirma se é para redefinir o ranking
void resetRanking ( HANDLE, ScreenResolution ); // Redefine o ranking
void checkTopScore ( HANDLE, ScreenResolution, int ); // Verifica se o jogador obteve uma pontuação que permita entrar no TOP
char* requestPlayerName ( HANDLE, ScreenResolution, char*, int ); // Pede ao jogador o nome que quer que apareca no ranking
void addTopScore ( HANDLE, PlayerScore [ ], int, int, int, const char* ); // Adiciona o jogador ao ranking

#endif	/* Not _RANKING_H_ */
