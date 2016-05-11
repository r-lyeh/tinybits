// Tiny tty writer: clamp-to-width console logger (only needed for win32)
// - rlyeh, public domain

#include <stdio.h>
#ifdef _WIN32
#include <string.h>
#include <winsock2.h>
#endif

void tty( const char *txt ) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO c;
    if( GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c) ) {
        int len = strlen(txt), w = c.srWindow.Right-c.srWindow.Left-c.dwCursorPosition.X;
        printf("%.*s%s\n", len > w ? w - 3 : w, txt, len > w ? "..." : "" );
        return;
    }
#endif
    puts( txt );
};

int main() {
    tty( "hey" );
    tty( "DECREASE WINDOW WIDTH AND RUN THE APP AGAIN: there is a lady whos sure all that glitters is gold, there is a lady whos sure all that glitters is gold, there is a lady whos sure all that glitters is gold" );
    printf("%s", "[test] ");
    tty( "DECREASE WINDOW WIDTH AND RUN THE APP AGAIN: there is a lady whos sure all that glitters is gold, there is a lady whos sure all that glitters is gold, there is a lady whos sure all that glitters is gold" );
}
