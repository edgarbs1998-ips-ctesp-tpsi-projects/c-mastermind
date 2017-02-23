#include <windows.h>
#include <stdio.h>
#include <locale.h>

#include "util.h"
#include "settings.h"
#include "colors.h"

// Declaração das funções

// Reproduz o BEEP passado por argumento, caso o som do jogo esteja ativado
void playBeep ( int enabled, UINT beep ) {
    if ( enabled ) {
        MessageBeep ( beep );
    }
}

// Funciona como um printf mas permite escolher as coordenadas onde o texto será apresentado
int printfXY ( HANDLE hStdout, int posX, int posY, const char* format, ... ) {
    va_list arg;
    int done;

    SetConsoleCursorPosition ( hStdout, ( COORD ) { posX, posY } );

    va_start ( arg, format );
    done = vfprintf ( stdout, format, arg );
    va_end ( arg );

    return done;
}

// Função para escrever um carater pertencente a tabela ASCII Extended
void printSpecialChar ( const char charCode ) {
    setlocale ( LC_ALL, "C" );

    putchar ( charCode );

    setlocale ( LC_ALL, APP_LANGUAGE );
}

// Função que muda a cor de fundo de todo o ecrã
void setBackgroundColor ( HANDLE hStdout, WORD bacgrkoundColor, WORD foregroundColor ) {
    COORD coordScreen = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    DWORD cCharsWritten;

    // Get the number of character cells in the current buffer
    if( !GetConsoleScreenBufferInfo ( hStdout, &csbi ) ) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputAttribute ( hStdout,         // Screen output handler
                                bacgrkoundColor,      // Character attributes to use
                                dwConSize,        // Number of cells to set attribute
                                coordScreen,      // Coordinates of first cell
                                &cCharsWritten ); // Receive number of characters written

    SetConsoleTextAttribute( hStdout, bacgrkoundColor | foregroundColor );
}

// Função que define as dimensões da janela de acordo com o tamanho passado por argumento
void setWindowSize ( HANDLE hStdout, DWORD sizeX, DWORD sizeY ) {
    COORD bufferSize;
    SMALL_RECT windowSize;

    bufferSize.X = sizeX;
    bufferSize.Y = sizeY;

    windowSize.Left = 0;
    windowSize.Top = 0;
    windowSize.Right = bufferSize.X - 1;
    windowSize.Bottom = bufferSize.Y - 1;

    //SetConsoleScreenBufferSize( hStdout, bufferSize );
    SetConsoleWindowInfo( hStdout, 1, &windowSize );
    SetConsoleScreenBufferSize( hStdout, bufferSize );
}

// Função que apaga o ecrã na vertical a partir de uma determinada coordenada
void clearScreenBufferAt ( HANDLE hStdout, int posY ) {
    COORD coordScreen = { 0, posY };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    DWORD cCharsWritten;

    // Get the number of character cells in the current buffer
    if( !GetConsoleScreenBufferInfo ( hStdout, &csbi ) ) {
        return;
    }

    dwConSize = csbi.dwSize.X * ( csbi.dwSize.Y - posY );

    // Fill the entire screen with blanks
    if( !FillConsoleOutputCharacter ( hStdout,        // Handle to console screen buffer
                                     ( TCHAR ) ' ',   // Character to write to the buffer
                                     dwConSize,       // Number of cells to write
                                     coordScreen,     // Coordinates of first cell
                                     &cCharsWritten ) // Receive number of characters written
       ) {
        return;
    }

    // Put the cursor at its home coordinates
    SetConsoleCursorPosition ( hStdout, coordScreen );
}

// Função que apaga o ecrã na totalidade, definindo o fundo do ecrã a preto e o texto a branco ( verificado ser mais eficiente que system("cls"); )
// Adaptada da página https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022(v=vs.85).aspx
void clearScreen ( HANDLE hStdout ) {
    COORD coordScreen = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    DWORD cCharsWritten;

    // Get the number of character cells in the current buffer
    if( !GetConsoleScreenBufferInfo ( hStdout, &csbi ) ) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks
    if( !FillConsoleOutputCharacter ( hStdout,        // Handle to console screen buffer
                                     ( TCHAR ) ' ',   // Character to write to the buffer
                                     dwConSize,       // Number of cells to write
                                     coordScreen,     // Coordinates of first cell
                                     &cCharsWritten ) // Receive number of characters written
       ) {
        return;
    }

    // Set the buffer's attributes accordingly
    if( !FillConsoleOutputAttribute ( hStdout,        // Handle to console screen buffer
                                     RESET_COLORS,            // Character attributes to use
                                     dwConSize,       // Number of cells to set attribute
                                     coordScreen,     // Coordinates of first cell
                                     &cCharsWritten ) // Receive number of characters written
       ) {
        return;
    }

    // Set foreground color do default (white)
    SetConsoleTextAttribute ( hStdout, FOREGROUND_WHITE );

    // Put the cursor at its home coordinates
    SetConsoleCursorPosition ( hStdout, coordScreen );
}
