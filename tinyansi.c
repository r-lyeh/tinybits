// Tiny terminal utilities. rlyeh, public domain
#include <stdio.h>

#ifdef _WIN32
#define ANSI(ansi, win) win
#else
#define ANSI(ansi, win) ansi
#endif

// A few unicode characters and ANSI colors
static const char *tick = ANSI("\u2713", "[v]"), *cross = ANSI("\u2717", "[x]"), *arrow = ANSI("\u2794", "->");
static const char *red = ANSI("\x1b[31m",""), *green = ANSI("\x1b[32m",""), *blue = ANSI("\x1b[34m","");
static const char *yellow = ANSI("\x1b[33m",""), *magenta = ANSI("\x1b[35m",""), *cyan = ANSI("\x1b[36m","");
static const char *end = ANSI("\x1b[0m","");

// 256-color terminal
void ttycolor( unsigned char r, unsigned char g, unsigned char b ) {
    ANSI(printf("\033[38;5;%dm", (r/51)*36+(g/51)*6+(b/51)+16), (void)0);
}


int main() {
    // usage:
    printf("%s%s%s%s%s\n", green, tick, yellow, " passed", end);
    // or:
    ttycolor( 255, 192, 0 );
    printf("%s\n", "256 colors");
}
