#ifndef	_UTIL_H_
#define	_UTIL_H_

#include <windows.h>

// Constantes relacionadas a teclas pressionadas
#define KEY_SPACE 32
#define KEY_ARROW_UP 72
#define KEY_ARROW_DOWN 80
#define KEY_ARROW_LEFT 75
#define KEY_ARROW_RIGHT 77

// Declaração da estrutura ScreenResolution para guardar a resolução do ecrã
typedef struct {
    DWORD width;
    DWORD height;
} ScreenResolution;

// Declaração dos protótipos das funções
void playBeep ( int, UINT ); // Reproduz o BEEP passado por argumento
int printfXY ( HANDLE, int, int, const char*, ... ); // Funciona como um printf mas permite escolher as coordenadas onde o texto será apresentado
void printSpecialChar ( const char ); // Função para escrever um carater pertencente a tabela ASCII Extended
void setBackgroundColor ( HANDLE, WORD, WORD ); // Função que muda a cor de fundo de todo o ecrã
void setWindowSize ( HANDLE, DWORD, DWORD ); // Função que define as dimensões da janela de acordo com o tamanho passado por argumento
void clearScreenBufferAt ( HANDLE, int ); // Função que apaga o ecrã na vertical a partir de uma determinada coordenada
void clearScreen ( HANDLE ); // Função que apaga o ecrã na totalidade ( mais eficiente que system("cls"); )

#endif	/* Not _UTIL_H_ */
