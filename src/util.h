#ifndef	_UTIL_H_
#define	_UTIL_H_

#include <windows.h>

// Constantes relacionadas a teclas pressionadas
#define KEY_SPACE 32
#define KEY_ARROW_UP 72
#define KEY_ARROW_DOWN 80
#define KEY_ARROW_LEFT 75
#define KEY_ARROW_RIGHT 77

// Declara��o da estrutura ScreenResolution para guardar a resolu��o do ecr�
typedef struct {
    DWORD width;
    DWORD height;
} ScreenResolution;

// Declara��o dos prot�tipos das fun��es
void playBeep ( int, UINT ); // Reproduz o BEEP passado por argumento
int printfXY ( HANDLE, int, int, const char*, ... ); // Funciona como um printf mas permite escolher as coordenadas onde o texto ser� apresentado
void printSpecialChar ( const char ); // Fun��o para escrever um carater pertencente a tabela ASCII Extended
void setBackgroundColor ( HANDLE, WORD, WORD ); // Fun��o que muda a cor de fundo de todo o ecr�
void setWindowSize ( HANDLE, DWORD, DWORD ); // Fun��o que define as dimens�es da janela de acordo com o tamanho passado por argumento
void clearScreenBufferAt ( HANDLE, int ); // Fun��o que apaga o ecr� na vertical a partir de uma determinada coordenada
void clearScreen ( HANDLE ); // Fun��o que apaga o ecr� na totalidade ( mais eficiente que system("cls"); )

#endif	/* Not _UTIL_H_ */
