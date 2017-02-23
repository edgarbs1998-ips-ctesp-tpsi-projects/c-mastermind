#ifndef	_MENU_H_
#define	_MENU_H_

#include <windows.h>

#include "util.h"

// Declara��o dos prot�tipos das fun��es
void displayMenu ( HANDLE, ScreenResolution ); // Mostra menu e permite escolher a op��o pertendida
void displayWelcome ( HANDLE, ScreenResolution ); // Mostra tela de boas-vindas
void displayInstructions ( HANDLE, ScreenResolution ); // Mostra as instru��es e regras do jogo

#endif	/* Not _MENU_H_ */
