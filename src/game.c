#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>

#include "game.h"
#include "settings.h"
#include "colors.h"
#include "util.h"
#include "ranking.h"

// Variável global para as cores usadas nas peças
const WORD PIECE_COLORS [ GAME_COLORS ] = {
    PIECE_FOREGROUND_GREEN,
    PIECE_FOREGROUND_CYAN,
    PIECE_FOREGROUND_MAGENTA,
    PIECE_FOREGROUND_YELLOW
};

// Variável global para o som do jogo
int gameSound = 1; // Ativado por predefinição

// Array global para a estrutura do quadro
char boardArray [ BOARD_SIZE_Y + 2 ] [ BOARD_SIZE_X + 1 ] = { { 0 } };

// Array global para as peças do jogo
int gameSecretKey [ GAME_PIECES ];

// Declaração das funções

// Prepara o jogo, chama a função para criar o quadro, gerar a chave secreta e iniciar o jogo
void prepareGame ( HANDLE hStdout, ScreenResolution screenRes ) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    clearScreen ( hStdout );

    setBackgroundColor ( hStdout, BOARD_COLOR, FOREGROUND_WHITE );

    // Desenha o quadro do jogo
    drawBoard ( hStdout, screenRes, BOARD_SIZE_X, BOARD_SIZE_Y );

    // Get the number of character cells in the current buffer
    GetConsoleScreenBufferInfo ( hStdout, &csbi );

    // Prepara a chave secreta
    prepareGameKey ( hStdout, screenRes, csbi, GAME_PIECES, 3, BOARD_SIZE_Y + 1 );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    SetConsoleTextAttribute ( hStdout, BOARD_COLOR | FOREGROUND_WHITE );
    printfXY ( hStdout, 1, BOARD_SIZE_Y + 4, "Pressione qualquer tecla\n\
              \r para iniciar o jogo !!" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, BOARD_SIZE_Y + 4 } );

    setWindowSize ( hStdout, csbi.dwSize.X + 6, csbi.dwSize.Y + 3 );

    // Espera que o utilizador pressione qualquer tecla para iniciar o jogo
    getch ( );

    clearScreenBufferAt ( hStdout, BOARD_SIZE_Y + 3 );
    setWindowSize ( hStdout, csbi.dwSize.X, csbi.dwSize.Y + 2 );

    // Inicia o algoritmo do jogo
    startGame ( hStdout, screenRes, csbi );
}

// Desenha o quadro do jogo no ecrã
void drawBoard ( HANDLE hStdout, ScreenResolution screenRes, int sizeX, int sizeY ) {
    int i;

    // Caso a Array com o quadro ainda não exista é criada, desta forma só se cria a Array do quadro a primeira vez que se joga
    if ( boardArray [ 0 ] [ 0 ] == 0 ) {
        // Inicializa a Array do quadro do jogo
        prepareBoardArray ( sizeX, sizeY );
    }

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    // Permite a escrita de carateres da tabela ASCII Extended
    setlocale ( LC_ALL, "C" );

    // Escreve a Array do quadro no ecrã
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, 1 } );
    for ( i = 0; i < sizeY + 2; ++i ) {
        printfXY ( hStdout, 1, i + 1, "%s", boardArray [ i ] );
    }

    setWindowSize ( hStdout, sizeX + 2, sizeY + 4 );

    // Resota o idioma da aplicação
    setlocale ( LC_ALL, APP_LANGUAGE );
}

// Prepara a Array do quadro do jogo
void prepareBoardArray ( int sizeX, int sizeY ) {
    int i, x;

    // Desenha a parte do topo e do fundo do quadro
    boardArray [ 0 ] [ 0 ] = BOARD_CHAR_TOP_LEFT;
    boardArray [ sizeY - 1 ] [ 0 ] = BOARD_CHAR_BOTTOM_LEFT;
    for ( i = 1; i < sizeX - 1; ++i ) {
        boardArray [ 0 ] [ i ] = BOARD_CHAR_TOP_BOTTOM;
        boardArray [ sizeY - 1 ] [ i ] = BOARD_CHAR_TOP_BOTTOM;
    }
    boardArray [ 0 ] [ i - GAME_PIECES - 3 ]  = BOARD_CHAR_CORNER_TOP;
    boardArray [ 0 ] [ i ]  = BOARD_CHAR_TOP_RIGHT;
    boardArray [ sizeY - 1 ] [ i - GAME_PIECES - 3 ]  = BOARD_CHAR_CORNER_BOTTOM;
    boardArray [ sizeY - 1 ] [ i ]  = BOARD_CHAR_BOTTOM_RIGHT;
    boardArray [ 0 ] [ i + 1 ] = '\0';
    boardArray [ sizeY - 1 ] [ i + 1 ] = '\0';

    // Desenha as laterais e o centro do quadro
    for ( i = 1; i < sizeY - 1; ++i ) {
        boardArray [ i ] [ 0 ] = BOARD_CHAR_SIDE;
        for ( x = 1; x < sizeX - 1; ++x ) {
            boardArray [ i ] [ x ] = ' ';
        }
        boardArray [ i ] [ x - GAME_PIECES - 3 ] = BOARD_CHAR_SIDE;
        boardArray [ i ] [ x ] = BOARD_CHAR_SIDE;
        boardArray [ i ] [ x + 1 ] = '\0';
    }

    // Desenha a parte da chave secreta
    boardArray [ sizeY ] [ 0 ] = BOARD_CHAR_SIDE;
    boardArray [ sizeY + 1 ] [ 0 ] = BOARD_CHAR_CORNER_LEFT;
    for ( x = 1; x < sizeX - GAME_PIECES - 4; ++x ) {
        boardArray [ sizeY ] [ x ] = ' ';
        boardArray [ sizeY + 1 ] [ x ] = BOARD_CHAR_TOP_BOTTOM;
    }
    boardArray [ sizeY ] [ x ] = BOARD_CHAR_SIDE;
    boardArray [ sizeY + 1 ] [ x ] = BOARD_CHAR_CORNER_RIGHT;
    boardArray [ sizeY ] [ x + 1 ] = '\0';
    boardArray [ sizeY + 1 ] [ x + 1 ] = '\0';
}

// Prepara a chave secreta
void prepareGameKey ( HANDLE hStdout, ScreenResolution screenRes, CONSOLE_SCREEN_BUFFER_INFO csbi, int pieces, int posX, int posY ) {
    int i, x;

    // Gera a chave secreta
    generateGameKey ( );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    SetConsoleTextAttribute ( hStdout, BOARD_COLOR | FOREGROUND_WHITE );
    printfXY ( hStdout, 1, posY + 3, "Preparando chave ..." );

    setWindowSize ( hStdout, csbi.dwSize.X + 2, csbi.dwSize.Y + 2 );

    // Em modo DEBUG a solução é apresentada de imediato, caso contrario é apresentada uma animação da geração da chave
    #if !DEBUG
    for ( i = 0; i < pieces; ++i ) {
        for ( x = 0; x < 3; ++x ) {
            SetConsoleCursorPosition ( hStdout, ( COORD ) { posX + ( i * 2 ), posY } );
            printSpecialChar ( ' ' );
            Sleep ( 200 );
            SetConsoleCursorPosition ( hStdout, ( COORD ) { posX + ( i * 2 ), posY } );
            printSpecialChar ( 254 );
            Sleep ( 200 );
        }

        playBeep ( gameSound, MB_ICONINFORMATION );

        Sleep ( 50 );
    }

    // Descarta todos os dados introduzidos durante as funções Sleep
    while ( kbhit ( ) ) {
        getch ( );
    }
    #else
    for ( i = 0; i < pieces; ++i ) {
        SetConsoleCursorPosition ( hStdout, ( COORD ) { posX + ( i * 2 ), posY } );
        SetConsoleTextAttribute ( hStdout, BOARD_COLOR | PIECE_COLORS [ gameSecretKey [ i ] ] );
        printSpecialChar ( 254 );
    }
    #endif // DEBUG

    clearScreenBufferAt ( hStdout, BOARD_SIZE_Y + 3 );
    setWindowSize ( hStdout, csbi.dwSize.X, csbi.dwSize.Y );
}

// Gera a chave secreta
void generateGameKey ( void ) {
    int i;

    for ( i = 0; i < GAME_PIECES; ++i ) {
        gameSecretKey [ i ] = rand ( ) % GAME_COLORS;
    }
}

// Inicia o algoritmo do jogo
void startGame ( HANDLE hStdout, ScreenResolution screenRes, CONSOLE_SCREEN_BUFFER_INFO csbi ) {
    int i, x, keyInput, attemptPieces [ GAME_PIECES ] = { 0 }, gameAttempt = 1, cursorPiece, confirmAttempt, blackPiece, whitePiece, checkSecret [ GAME_PIECES ], checkAttempt [ GAME_PIECES ];

    // Ciclo para cada jogada
    while ( gameAttempt <= GAME_ATTEMPTS ) {
        playBeep ( gameSound, MB_ICONWARNING );

        SetConsoleTextAttribute ( hStdout, BOARD_COLOR | FOREGROUND_WHITE );
        clearScreenBufferAt ( hStdout, BOARD_SIZE_Y + 4 );
        printfXY ( hStdout, 1, BOARD_SIZE_Y + 4, "%d tent. restante", GAME_ATTEMPTS - gameAttempt + 1 );
        if ( GAME_ATTEMPTS - gameAttempt + 1 != 1 ) {
            printf ( "s" );
        }

        // Mostra as peças da jogada anterior
        for ( i = 0; i < GAME_PIECES; ++i ) {
            SetConsoleTextAttribute ( hStdout, BOARD_COLOR | PIECE_COLORS [ attemptPieces [ i ] ] );
            SetConsoleCursorPosition ( hStdout, ( COORD ) { 1 + ( i + 1 ) * 2, 1 + gameAttempt } );
            printSpecialChar ( 254 );
        }

        // Define as variáveis de controlo a 0 ou 1
        blackPiece = 0;
        whitePiece = 0;
        confirmAttempt = 0;
        cursorPiece = 0;

        for ( i = 0; i < GAME_PIECES; ++i ) {
            checkSecret [ i ] = 1;
            checkAttempt [ i ] = 1;
        }

        SetConsoleCursorPosition ( hStdout, ( COORD ) { 1 + ( cursorPiece + 1 ) * 2, 1 + gameAttempt } );

        // Ciclo do algoritmo de seleção de peça até confirmar a tentativa
        do {
            // Guarda a tecla pressionada
            keyInput = getch ( );
            // Se for uma seta direcional vai chamar outra vez a função getch(); pois as setas geram dois eventos em vez de um como no caso dos restantes carateres
            if ( keyInput == 0 /* NUMPAD Arrows */ || keyInput == 224 /* NORMAL Arrows */ ) {
                keyInput = getch ( ); // Obtém a seta direcional pressionada
            }
            else {
                // Se pressionar a tecla espaço a jogada atual é confirmada
                if ( keyInput == KEY_SPACE ) {
                    confirmAttempt = 1;
                }

                continue;
            }

            // Verifica a tecla direcional pressionada
            switch ( keyInput ) {
                // Seta para cima muda a cor da peça, ao alcançar o fim volta ao inicio
                case KEY_ARROW_UP:
                    if ( attemptPieces [ cursorPiece ] == GAME_COLORS - 1 ) {
                        attemptPieces [ cursorPiece ] = 0;
                    }
                    else {
                        ++attemptPieces [ cursorPiece ];
                    }

                    SetConsoleTextAttribute ( hStdout, BOARD_COLOR | PIECE_COLORS [ attemptPieces [ cursorPiece ] ] );
                    printSpecialChar ( 254 );
                    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1 + ( cursorPiece + 1 ) * 2, 1 + gameAttempt } );

                    break;

                // Seta para baixo muda a cor da peça no sentido inverso, ao alcançar o fim volta ao inicio
                case KEY_ARROW_DOWN:
                    if ( attemptPieces [ cursorPiece ] == 0 ) {
                        attemptPieces [ cursorPiece ] = GAME_COLORS - 1;
                    }
                    else {
                        --attemptPieces [ cursorPiece ];
                    }

                    SetConsoleTextAttribute ( hStdout, BOARD_COLOR | PIECE_COLORS [ attemptPieces [ cursorPiece ] ] );
                    printSpecialChar ( 254 );
                    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1 + ( cursorPiece + 1 ) * 2, 1 + gameAttempt } );

                    break;

                // Seta para a esquerda muda a seleção da peça, ao alcançar o fim volta ao inicio
                case KEY_ARROW_LEFT:
                    if ( cursorPiece == 0 ) {
                        cursorPiece = GAME_PIECES - 1;
                    }
                    else {
                        --cursorPiece;
                    }

                    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1 + ( cursorPiece + 1 ) * 2, 1 + gameAttempt } );

                    break;

                // Seta para a esquerda muda a seleção da peça no sentido inverso, ao alcançar o fim volta ao inicio
                case KEY_ARROW_RIGHT:
                    if ( cursorPiece == GAME_PIECES - 1 ) {
                        cursorPiece = 0;
                    }
                    else {
                        ++cursorPiece;
                    }

                    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1 + ( cursorPiece + 1 ) * 2, 1 + gameAttempt } );

                    break;
            }
        } while ( !confirmAttempt ); // Verifica se a jogada foi confirmada

        // Verifica se as peças da combinação introduzida estão corretas e na posição correta e adiciona um pino preto em caso positivo
        for ( i = 0; i < GAME_PIECES; ++i ) {
            if ( gameSecretKey [ i ] == attemptPieces [ i ] ) {
                SetConsoleCursorPosition ( hStdout, ( COORD ) { GAME_PIECES * 2 + 5 + blackPiece, 1 + gameAttempt } );
                SetConsoleTextAttribute ( hStdout, BOARD_COLOR );
                printSpecialChar ( 254 );

                ++blackPiece;

                checkSecret [ i ] = 0;
                checkAttempt [ i ] = 0;
            }
        }

        // Verifica se as restantes peças da combinação introduzida estão corretas e adiciona um pino branco em caso positivo
        for ( i = 0; i < GAME_PIECES; ++i ) {
            for ( x = 0; x < GAME_PIECES; ++x ) {
                if ( gameSecretKey [ i ] == attemptPieces [ x ] && checkSecret [ x ] && checkAttempt [ i ] && i != x ) {
                    SetConsoleCursorPosition ( hStdout, ( COORD ) { GAME_PIECES * 2 + 5 + blackPiece + whitePiece, 1 + gameAttempt } );
                    SetConsoleTextAttribute ( hStdout, BOARD_COLOR | FOREGROUND_WHITE );
                    printSpecialChar ( 254 );

                    ++whitePiece;

                    checkSecret [ x ] = 0;
                    checkAttempt [ i ] = 0;
                }
            }
        }

        // Verifica se a combinação está correta
        if ( blackPiece == GAME_PIECES ) {
            playBeep ( gameSound, MB_OK );

            // Mostra o ecrã de vitoria
            displayWinScreen ( hStdout, screenRes, gameAttempt );

            // Verifica se o jogador pode entrar para o ranking
            checkTopScore ( hStdout, screenRes, gameAttempt );

            return;
        }

        // Avança para a proxima tentativa
        ++gameAttempt;
    }

    playBeep ( gameSound, MB_ICONERROR );

    // Caso o jogador não encontre a combinação correta após o limite de tentativas a solução é apresentada no ecrã
    for ( i = 0; i < GAME_PIECES; ++i ) {
        SetConsoleCursorPosition ( hStdout, ( COORD ) { 1 + ( i + 1 ) * 2, BOARD_SIZE_Y + 1 } );
        SetConsoleTextAttribute ( hStdout, BOARD_COLOR | PIECE_COLORS [ gameSecretKey [ i ] ] );
        printSpecialChar ( 254 );
    }

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    SetConsoleTextAttribute ( hStdout, BOARD_COLOR | FOREGROUND_WHITE );
    clearScreenBufferAt ( hStdout, BOARD_SIZE_Y + 4 );

    printfXY ( hStdout, 1, BOARD_SIZE_Y + 4, "%d tent. restantes", GAME_ATTEMPTS - gameAttempt + 1 );

    printfXY ( hStdout, 1, BOARD_SIZE_Y + 6, "Pressione qualquer\n\
              \r tecla para continuar!" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, BOARD_SIZE_Y + 6 } );

    setWindowSize ( hStdout, csbi.dwSize.X + 3, csbi.dwSize.Y + 5 );

    // Aguarda que o jogador pressiona qualquer tecla para poder ver a combinação correta
    getch ( );

    // Apresenta o ecrã de perdedor
    displayLossScreen ( hStdout, screenRes );
}

// Apresenta o ecrã de vitoria no caso de acerta na combinação antes de terminar as tentativas
void displayWinScreen ( HANDLE hStdout, ScreenResolution screenRes, int gameAttempts ) {
    const char asciiWinText [ ] = "\n\
          _____                 _\n\
         |  __ \\               | |\n\
         | |__) |__ _ _ __ __ _| |__   ___ _ __  ___\n\
         |  ___// _` | '__/ _` | '_ \\ / _ \\ '_ \\/ __|\n\
         | |   | (_| | | | (_| | |_) |  __/ | | \\__ \\\n\
         |_|    \\__,_|_|  \\__,_|_.__/ \\___|_| |_|___/";

    clearScreen ( hStdout );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_GREEN_INTENSE );
    printf ( asciiWinText );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
    printfXY ( hStdout, 2, 10, "Parabéns !!!  Você descobriu a combinação em %d tentativa", gameAttempts );
    if ( gameAttempts != 1 ) {
        printf ( "s." );
    }
    else {
        printf ( "." );
    }

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printfXY ( hStdout, 20, 14, "Pressione qualquer tecla" );
    printfXY ( hStdout,  24, 15, "para continuar !" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 20, 14 } );

    setWindowSize ( hStdout, 64, 17 );

    getch ( );
}

// Apresenta o ecrã de perdador no caso de não encontrar a solução no limite de tentativas
void displayLossScreen ( HANDLE hStdout, ScreenResolution screenRes ) {
    const char asciiLossText [ ] = "\n\
         _____              _\n\
        |  __ \\            | |\n\
        | |__) |___ _ __ __| | ___ _   _\n\
        |  ___// _ \\ '__/ _` |/ _ \\ | | |\n\
        | |   |  __/ | | (_| |  __/ |_| |\n\
        |_|    \\___|_|  \\__,_|\\___|\\__,_|";

    clearScreen ( hStdout );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printf ( asciiLossText );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
    printfXY ( hStdout, 3, 10, "Você usou todas as tentativas disponíveis e" );
    printfXY ( hStdout, 2, 11, "não conseguiu encontrar a combinação correta." );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printfXY ( hStdout, 12, 15, "Pressione qualquer tecla" );
    printfXY ( hStdout,  16, 16, "para continuar !" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 12, 15 } );

    setWindowSize ( hStdout, 49, 18 );

    getch ( );
}
