// Tiny atoi() replacement. rlyeh, public domain
#pragma once
int tinyatoi( const char *s ) {
    int v = 0, n = 1;
    if( s ) {
        while( *s == '-' ) n *= -1, s++;
        while( *s >= '0' && *s <= '9') v = (v * 10) + *s++ - '0';
    }
    return n * v;
}

#ifdef TINYATOI_MAIN
#include <stdio.h>
int TINYATOI_MAIN() {
    printf("%d\n", tinyatoi("01230"));
    printf("%d\n", tinyatoi("-01230"));
    printf("%d\n", tinyatoi("--01230"));
    printf("%d\n", tinyatoi("---01230"));
}
#endif
