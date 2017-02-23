#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "ranking.h"
#include "colors.h"
#include "util.h"

// Declaração das funções

// Mostra as melhores jogadas guardadas no ficheiro
void displayRanking ( HANDLE hStdout, ScreenResolution screenRes ) {
    FILE *ranking;
    PlayerScore player [ RANK_TOP_SIZE ];
    int i, players = 0;

    clearScreen ( hStdout );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    ranking = fopen ( FILE_RANKING_PATH, "r" );

    if ( !ranking ) {
        emptyRanking ( hStdout, screenRes );

        return;
    }

    while ( fscanf ( ranking, "%d-%d-%d %d:%d:%d %d %" MAX_PLAYER_NAME_STRING "[^\n]", &player [ players ].time.tm_mday,
                                                                                       &player [ players ].time.tm_mon,
                                                                                       &player [ players ].time.tm_year,
                                                                                       &player [ players ].time.tm_hour,
                                                                                       &player [ players ].time.tm_min,
                                                                                       &player [ players ].time.tm_sec,
                                                                                       &player [ players ].score,
                                                                                       player [ players ].name )!= EOF && players < RANK_TOP_SIZE ) {
        ++players;
    }

    fclose ( ranking );

    if ( players == 0 ) {
        emptyRanking ( hStdout, screenRes );

        return;
    }

    SetConsoleTextAttribute ( hStdout, FOREGROUND_CYAN_INTENSE );
    printf ( "\n\
            \r ===================================\n\
            \r | Mastermind - Ranking ( TOP 10 ) |\n\
            \r ===================================\n" );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
    printfXY ( hStdout, 1, 5, "Posição | Data      \t| Hora    \t| Nome                \t| Tentativas" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, 6 } );
    for ( i = 0; i < 75; ++i ) {
        putchar ( '-' );
    }
    for ( i = 0; i < players; ++i ) {
        printfXY ( hStdout, 1, 7 + i, "%-7d | %02d-%02d-%d\t| %02d:%02d:%02d\t| %-" MAX_PLAYER_NAME_STRING "s\t| %d\n", i + 1, player [ i ].time.tm_mday,
                                                                                                                               player [ i ].time.tm_mon,
                                                                                                                               player [ i ].time.tm_year,
                                                                                                                               player [ i ].time.tm_hour,
                                                                                                                               player [ i ].time.tm_min,
                                                                                                                               player [ i ].time.tm_sec,
                                                                                                                               player [ i ].name,
                                                                                                                               player [ i ].score );
    }

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printfXY ( hStdout, 1, 8 + players + 1, "Pressione qualquer tecla para continuar!" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, 8 + players + 1 } );

    setWindowSize ( hStdout, 77, 8 + players + 3 );

    getch ( );
}

// Mostra uma mensagem de informativa no caso de não existirem jogadas para apresentar
void emptyRanking ( HANDLE hStdout, ScreenResolution screenRes ) {
    clearScreen ( hStdout );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
    printfXY ( hStdout, 1, 1, "Ainda não existem registos de jogos!" );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printfXY ( hStdout, 1, 3, "Prima qualquer tecla para continuar!" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, 3 } );

    setWindowSize ( hStdout, 38, 5 );

    getch ( );
}

// Confirma se é para redefinir o ranking
void confirmResetRanking ( HANDLE hStdout, ScreenResolution screenRes ) {
    char menuOption;

    clearScreen ( hStdout );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_CYAN_INTENSE );
    printf ( "\n\
            \r ==================================\n\
            \r | Mastermind - Redefinir Ranking |\n\
            \r ==================================\n" );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
    printf ( "\n Tem a certeza que quer redefinir o ranking?\n" );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
    printf ( "\n 1 - Sim\n\
            \r 2 - Não\n" );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printf ( "\n\n Aviso: " );
    SetConsoleTextAttribute ( hStdout, FOREGROUND_GRAY );
    printf ( "Esta ação não poderá ser desfeita!" );

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printf ( "\n\n Escolha uma opção!" );
    SetConsoleCursorPosition ( hStdout,  ( COORD ) { 1, 13 } );

    setWindowSize ( hStdout, 45, 15 );

    while ( 1 ) {
        // Aguarda que o utilizador pressione um carater, ignora a tecla ENTER que causava comportamentos indesejados
        do {
            menuOption = getch ( );
        } while ( menuOption == 13 /* RETURN */ );

        switch ( menuOption ) {
            case '1':
                resetRanking ( hStdout, screenRes );

                return;

            case '2':
                return;
        }
    }
}

// Redefine o ranking
void resetRanking ( HANDLE hStdout, ScreenResolution screenRes ) {
    int success;

    clearScreen ( hStdout );

    setWindowSize ( hStdout, screenRes.width, screenRes.height );

    success = remove ( FILE_RANKING_PATH );

    if ( success ) {
        SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
        printfXY ( hStdout, 1, 1, "Falhou a redefinir o ranking ou não existia qualquer registo de jogos!" );
    }
    else {
        SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
        printfXY ( hStdout, 1, 1, "O ranking foi redefinido com sucesso!" );
    }

    SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
    printfXY ( hStdout, 1, 3, "Prima qualquer tecla para continuar!" );
    SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, 3 } );

    if ( success ) {
        setWindowSize ( hStdout, 72, 5 );
    }
    else {
        setWindowSize ( hStdout, 39, 5 );
    }

    getch ( );
}

// Verifica se o jogador obteve uma pontuação que permita entrar no TOP
void checkTopScore ( HANDLE hStdout, ScreenResolution screenRes, int attempts ) {
    FILE *ranking;
    PlayerScore player [ RANK_TOP_SIZE ];
    int players = 0, i = 0;
    char name [ MAX_PLAYER_NAME ];

    ranking = fopen ( FILE_RANKING_PATH, "r" );

    // Caso o ficheiro não exista adiciona o jogador ao TOP sem qualquer verificação
    if ( !ranking ) {
        requestPlayerName ( hStdout, screenRes, name, MAX_PLAYER_NAME - 1 );

        if ( name [ 0 ] != '\0' ) {
            addTopScore ( hStdout, player, players, 0, attempts, name );
        }

        return;
    }

    // Carrega para a estrutura PlayerScore as jogadas presentes no ficheiro
    while ( fscanf ( ranking, "%d-%d-%d %d:%d:%d %d %" MAX_PLAYER_NAME_STRING "[^\n]", &player [ players ].time.tm_mday,
                                                                                       &player [ players ].time.tm_mon,
                                                                                       &player [ players ].time.tm_year,
                                                                                       &player [ players ].time.tm_hour,
                                                                                       &player [ players ].time.tm_min,
                                                                                       &player [ players ].time.tm_sec,
                                                                                       &player [ players ].score,
                                                                                       player [ players ].name ) != EOF && players < RANK_TOP_SIZE ) {
        ++players;
    }

    fclose ( ranking );

    // Caso o ficheiro não possua dados adiciona o jogador ao TOP sem qualquer verificação adicional
    if ( !players ) {
        requestPlayerName ( hStdout, screenRes, name, MAX_PLAYER_NAME - 1 );

        if ( name [ 0 ] != '\0' ) {
            addTopScore ( hStdout, player, players, 0, attempts, name );
        }

        return;
    }

    // Verifica se a pontuação do jogador permite entrar no TOP
    for ( i = 0; i < players; ++i ) {
        if ( attempts < player [ i ].score ) {
            requestPlayerName ( hStdout, screenRes, name, MAX_PLAYER_NAME - 1 );

            if ( name [ 0 ] != '\0' ) {
                addTopScore ( hStdout, player, players, i, attempts, name );
            }

            break;
        }
        else if ( i == players - 1 && i < RANK_TOP_SIZE - 1 ) {
            requestPlayerName ( hStdout, screenRes, name, MAX_PLAYER_NAME - 1 );

            if ( name [ 0 ] != '\0' ) {
                addTopScore ( hStdout, player, players, i + 1, attempts, name );
            }

            break;
        }
    }
}

// Pede ao jogador o nome que quer que apareca no ranking
char* requestPlayerName ( HANDLE hStdout, ScreenResolution screenRes, char* name, int maxNameLen ) {
    char tempName [ maxNameLen ];

    do {
        clearScreen ( hStdout );

        setWindowSize ( hStdout, screenRes.width, screenRes.height );

        SetConsoleTextAttribute ( hStdout, FOREGROUND_GREEN_INTENSE );
        printfXY ( hStdout, 1, 1, "Parabéns, a sua pontuação atual permite-lhe entrar no ranking dos TOP %d.", RANK_TOP_SIZE );

        SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
        printfXY ( hStdout, 1, 3, "Introduza o seu nome caso queira aparecer no ranking:" );

        SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
        printfXY ( hStdout, 1, 8, "Notas:" );
        SetConsoleTextAttribute ( hStdout, FOREGROUND_GRAY );
        printfXY ( hStdout, 2, 9, "No caso de não querer aparecer no ranking deixe o seu nome em branco;" );
        printfXY ( hStdout, 2, 10, "Para confirmar a introdução do nome pressione a tecla ENTER;" );
        printfXY ( hStdout, 2, 11, "O tamanho máximo do nome é de 20 carateres." );

        SetConsoleTextAttribute ( hStdout, FOREGROUND_BLUE_INTENSE );
        SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, 5 } );

        setWindowSize ( hStdout, 76, 13 );

        gets ( tempName );

        if ( strlen ( tempName ) >  maxNameLen ) {
            setWindowSize ( hStdout, screenRes.width, screenRes.height );

            SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
            printfXY ( hStdout, 1, 14, "Erro: O nome introduzido não pode ser maior que 20 carateres!" );
            SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );
            printfXY ( hStdout, 1, 16, "Prima qualquer tecla para introduzir novamente o nome!" );
            SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, 16 } );

            setWindowSize ( hStdout, 76, 18 );

            getch ( );
        }
    } while ( strlen ( tempName ) >  maxNameLen );

    strcpy ( name, tempName );

    return name;
}

// Adiciona o jogador ao ranking
void addTopScore ( HANDLE hStdout, PlayerScore player [ ], int players, int index, int attempts, const char* name ) {
    FILE *ranking;
    time_t rawtime;
    int i;

    // Ajusta a variável player para o número total de registos, incluindo o que está a ser criado
    if ( players + 1 <= RANK_TOP_SIZE && players != 0 ) {
        ++players;
    }

    // Move as pontuações um índice acima a partir do índice onde vai adicionar a nova, descartando a ultima pontuação caso já não exista espaço no TOP
    for ( i = players - 1; i > index; --i ) {
        player [ i ] = player [ i - 1 ];
    }

    // Inicializa a estrutura com os dados pertendidos
    time ( &rawtime );
    player [ index ].time = *localtime ( &rawtime );
    player [ index ].time.tm_mon += 1;
    player [ index ].time.tm_year += 1900;
    player [ index ].score = attempts;
    strcpy ( player [ index ].name, name );

    // Abre o ficheiro para escrita
    ranking = fopen ( FILE_RANKING_PATH, "w" );

    // Apresenta uma mensagem de erro e aborta a criação do ranking caso não seja possível escrever no ficheiro
    if ( !ranking ) {
        clearScreen ( hStdout );

        SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
        printfXY ( hStdout, 1, 1, "Erro: A gravação da pontuação falhou!" );

        SetConsoleTextAttribute ( hStdout, FOREGROUND_RED_INTENSE );
        printfXY ( hStdout, 1, 3, "Prima qualquer tecla para continuar!" );
        SetConsoleCursorPosition ( hStdout, ( COORD ) { 1, 3 } );

        getch ( );

        return;
    }

    // Caso seja o primeiro registo adiciona 1 ao contador
    if ( !players ) {
        ++players;
    }

    // Reescreve todos os registos no ficheiro
    for ( i = 0; i < players; ++i ) {
        fprintf ( ranking, "%02d-%02d-%d %02d:%02d:%02d %d %s\n", player [ i ].time.tm_mday, player [ i ].time.tm_mon, player [ i ].time.tm_year, player [ i ].time.tm_hour, player [ i ].time.tm_min, player [ i ].time.tm_sec, player [ i ].score, player [ i ].name );
    }

    fclose ( ranking );
}
