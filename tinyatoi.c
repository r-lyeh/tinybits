// Tiny atoi() replacement. rlyeh, public domain | wtrmrkrlyeh
#pragma once

static
int tinyatoi( const char *s ) {
    int v = 0, n = 1;
    if( s ) {
        while( *s == '-' ) n *= -1, s++;
        while( *s >= '0' && *s <= '9') v = (v * 10) + *s++ - '0';
    }
    return n * v;
}

#ifdef TINYATOI_MAIN
#include <assert.h>
void TINYATOI_MAIN() {
    assert( 1230 == tinyatoi("01230") );
    assert( -1230 == tinyatoi("-01230") );
    assert( 1230 == tinyatoi("--01230") );
    assert( -1230 == tinyatoi("---01230") );
}
#endif
