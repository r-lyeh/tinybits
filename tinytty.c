// Tiny terminal utilities. rlyeh, public domain | wtrmrkrlyeh
#include <stdio.h>

#ifdef _WIN32
#define ANSI(ansi, win) win
#else
#define ANSI(ansi, win) ansi
#endif

// A few unicode characters and ANSI colors
static const char *tick   = ANSI("\u2713", "[v]"), *cross   = ANSI("\u2717", "[x]"), *arrow = ANSI("\u2794", "[>]");
static const char *red    = ANSI("\27[31m",""),    *green   = ANSI("\27[32m",""),    *blue = ANSI("\27[34m","");
static const char *yellow = ANSI("\27[33m",""),    *magenta = ANSI("\27[35m",""),    *cyan = ANSI("\27[36m","");
static const char *end    = ANSI("\27[0m","");

// 256-color terminal
void tty256( unsigned char r, unsigned char g, unsigned char b ) {
    ANSI(printf("\033[38;5;%dm", (r/51)*36+(g/51)*6+(b/51)+16), (void)0);
}

// terminal writer w/ console width clamping
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
}

/*
int main() {
    // usage:
    printf("%s%s%s%s%s\n", green, tick, yellow, " passed", end);
    // or:
    tty256( 255, 192, 0 );
    printf("%s\n", "256 colors");
    // also:
    tty( "DECREASE WINDOW WIDTH AND RUN THE APP AGAIN! DECREASE WINDOW WIDTH AND RUN THE APP AGAIN! DECREASE WINDOW WIDTH AND RUN THE APP AGAIN! DECREASE WINDOW WIDTH AND RUN THE APP AGAIN! " );
    // // more tests:
    // tty( "hey" );
    // printf("%s", "[test] ");
    // tty( "DECREASE WINDOW WIDTH AND RUN THE APP AGAIN! DECREASE WINDOW WIDTH AND RUN THE APP AGAIN! DECREASE WINDOW WIDTH AND RUN THE APP AGAIN! DECREASE WINDOW WIDTH AND RUN THE APP AGAIN! " );
}
*/
