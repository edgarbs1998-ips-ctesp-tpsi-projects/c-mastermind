#ifndef	_MENU_H_
#define	_MENU_H_

#include <windows.h>

#include "util.h"

// Declaração dos protótipos das funções
void displayMenu ( HANDLE, ScreenResolution ); // Mostra menu e permite escolher a opção pertendida
void displayWelcome ( HANDLE, ScreenResolution ); // Mostra tela de boas-vindas
void displayInstructions ( HANDLE, ScreenResolution ); // Mostra as instruções e regras do jogo

#endif	/* Not _MENU_H_ */
