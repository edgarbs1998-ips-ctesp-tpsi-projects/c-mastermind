#ifdef _WIN32
    #include <windows.h>
#else
    #error "Unsupported Operating System ( At this development point only Windows is supported )"
#endif

#include <stdio.h>
#include <locale.h>
#include <time.h>

#include "settings.h"
#include "colors.h"
#include "util.h"
#include "menu.h"
#include "game.h"
#include "ranking.h"

int main ( void ) {
    ScreenResolution screenRes;
    HANDLE hStdout;

    // Define o idioma do texto
    setlocale ( LC_ALL, APP_LANGUAGE );

    // Inicializa o gerador de números aleatórios
    srand ( ( unsigned ) time ( NULL ) );

    // Guarda nas variáveis a atual resolução do ecrã
    screenRes.width = GetSystemMetrics ( SM_CXSCREEN );
    screenRes.height = GetSystemMetrics ( SM_CYSCREEN );

    // Guarda nas variáveis os HANDLERS de saida
    hStdout = GetStdHandle ( STD_OUTPUT_HANDLE );

    // Define o titulo da consola
    SetConsoleTitle( GAME_TITLE );

    // Apresenta o ecrã de boas-vindas
    displayWelcome ( hStdout, screenRes );

    // Reproduz um som ao mostrar o menu pela primeria vez
    playBeep ( gameSound, MB_ICONWARNING );

    // Apresenta o menu
    displayMenu ( hStdout, screenRes );

    // Fecha os HANDLERS de saida
    CloseHandle ( hStdout );

    return 0;
}
