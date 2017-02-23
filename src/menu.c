#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "menu.h"
#include "colors.h"
#include "util.h"
#include "game.h"
#include "ranking.h"

// Declaração das funções

// Mostra menu e permite escolher a opção pertendida
void displayMenu ( HANDLE hStdout, ScreenResolution screenRes ) {
    char menuOption;
    int drawMenu = 1;

    while ( 1 ) {
        // Apenas escreve o menu caso seja necessário
        if ( drawMenu ) {
            clearScreen ( hStdout );

            setWindowSize ( hStdout, screenRes.width, screenRes.height );

            SetConsoleTextAttribute ( hStdout, FOREGROUND_CYAN_INTENSE );
            printf ( "\n\
                    \r =====================\n\
                    \r | Mastermind - Menu |\n\
                    \r =====================\n" );

            SetConsoleTextAttribute ( hStdout, FOREGROUND_GREEN_INTENSE );
            printf ( "\n 1 - Jogar\n" );

            SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
            printf ( " 2 - Ranking\n\
                    \r 3 - Redefinir Ranking\n\
                    \r 4 - Instruções\n\
                    \r 5 - Som: %s\n\
                    \r 6 - Sobre\n\
                    \n\
                    \r 0 - Sair", ( gameSound ? "Ligado" : "Desligado" ) );

            SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
            printf ( "\n\n\n Escolha uma opção!" );
            SetConsoleCursorPosition ( hStdout,  ( COORD ) { 1, 15 } );

            setWindowSize ( hStdout, 23, 17 );
        }

        // Aguarda que o utilizador pressione um carater, ignora a tecla ENTER que causava comportamentos indesejados
        do {
            menuOption = getch ( );
        } while ( menuOption == 13 /* RETURN */ );

        drawMenu = 1;
        switch ( menuOption ) {
            case '1':
                prepareGame ( hStdout, screenRes );

                break;

            case '2':
                displayRanking ( hStdout, screenRes );

                break;

            case '3':
                confirmResetRanking ( hStdout, screenRes );

                break;

            case '4':
                displayInstructions ( hStdout, screenRes );

                break;

            case '5':
                // Liga ou desliga o som do jogo, reproduz um BEEP quando é ativado
                if ( gameSound ) {
                    gameSound = 0;
                }
                else {
                    gameSound = 1;
                    playBeep ( gameSound, MB_ICONINFORMATION );
                }

                break;

            case '6':
                displayWelcome ( hStdout, screenRes );

                break;

            case '0':
                return;

            default:
                drawMenu = 0;
        }
    }
}

// Mostra tela de boas-vindas
void displayWelcome ( HANDLE hStdout, ScreenResolution screenRes ) {
    const char asciiTitle [ ] = "\n\
       __  __           _                      _           _\n\
      |  \\/  |         | |                    (_)         | |\n\
      | \\  / | __ _ ___| |_ ___ _ __ _ __ ___  _ _ __   __| |\n\
      | |\\/| |/ _` / __| __/ _ \\ '__| '_ ` _ \\| | '_ \\ / _` |\n\
      | |  | | (_| \\__ \\ ||  __/ |  | | | | | | | | | | (_| |\n\
      |_|  |_|\\__,_|___/\\__\\___|_|  |_| |_| |_|_|_| |_|\\__,_|\n";

    clearScreen ( hStdout );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_CYAN_INTENSE );
    printf ( asciiTitle );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );

    printf ( "\n\n  Este jogo foi desenvolvido no âmbito de um projeto universitário\n\
            \r e tem como objetivo demonstrar os conhecimentos adquiridos ao longo\n\
            \r do semestre na linguagem de programação C." );

    printf ( "\n\n  Desenvolvido por:\n\
            \r   - Edgar Santos\n\
            \r   - Miguel Rodrigues" );

    printf ( "\n\n  Instituição:\n\
            \r   - Instituto Politécnico de Setúbal" );

    printf ( "\n\n  Desenvolvido em 2016/2017" );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printfXY ( hStdout, 45, 15, "Pressione qualquer" );
    printfXY ( hStdout,  49, 16, "tecla para" );
    printfXY ( hStdout, 48, 17, "continuar !!" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 45, 15 } );

    setWindowSize ( hStdout, 69, 22 );

    getch ( );
}

// Mostra as instruções e regras do jogo
void displayInstructions ( HANDLE hStdout, ScreenResolution screenRes ) {
    clearScreen ( hStdout );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_CYAN_INTENSE );
    printf ( "\n\
            \r ===========================\n\
            \r | Mastermind - Instruções |\n\
            \r ===========================\n" );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
    printf ( "\n\
            \r  O Mastermind tem como objetivo descobrir uma combinação\n\
            \r de cores e as suas posições numa linha de 4 pinos em que\n\
            \r estamos a jogar contra o computador.\n\
            \n\
            \r  No início de cada partida, o computador deverá colocar\n\
            \r aleatoriamente 4 peças da esquerda para a direita,\n\
            \r utilizando as cores verde, ciano, magenta e amarelo.\n\
            \n\
            \r  O jogador terá um máximo de 10 tentativas para descobrir\n\
            \r a combinação. Em cada tentativa o jogador deverá indicar\n\
            \r a cor de 4 pinos. O número de tentativas restantes\n\
            \r será atualizado conforme vai jogando.\n\
            \n\
            \r  A cada tentativa o computador responderá com um pino\n\
            \r preto por cada cor certa no lugar certo e um pino branco\n\
            \r por cada cor certa no lugar errado.\n\
            \n\
            \r  Ao fim de cada jogo, caso a sua pontuação esteja entre\n\
            \r as 10 melhores, irá ser-lhe pedido que introduza o seu\n\
            \r nome para aparecer no ranking do jogo. No menu principal\n\
            \r terá uma opção para redefinir o ranking do jogo.\n\
            \n\
            \r Notas:\n\
            \r  - Para mudar as cores das peças use as setas direcionais\n\
            \r  para cima e para baixo;\n\
            \r  - Para mudar a peça selecionada use as setas direcionais\n\
            \r  para os lados;\n\
            \r  - Para confirmar a jogada atual pressione espaço." );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printfXY ( hStdout, 1, 35, "Pressione qualquer tecla para continuar!" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, 35 } );

    setWindowSize ( hStdout, 59, 37 );

    getch ( );
}
