// hexBASE 
// Very simple binary to ascii encoding. 0 to 2+100% bytes overhead (worst case)
//
// Specification:
// if char in [32..126] range then print "%c", char ('~' escaped as "~~"),
// else print "~%02x[...]~", for all remaining bytes.
// 
// - rlyeh, public domain.

#pragma once
#include <stdio.h>

static inline
void hexbasenl( FILE *fp, const void *ptr, int len ) {
    const unsigned char *buf = (const unsigned char*)ptr;
    for( ; len-- > 0; buf++ ) {
        unsigned char chr = (unsigned char)*buf;
        /**/ if( chr >= 32 && chr < 126 ) fprintf(fp, "%c", chr);
        else if( chr == 126 )             fprintf(fp, "%s", "~~"); 
        else {
            fprintf(fp, "%c", '~');
            do fprintf(fp, "%02x", *buf++); while( len-- > 0 );
            fprintf(fp, "%c", '~');
        }
    }
}

static inline
void hexbase( FILE *fp, const void *ptr, int len ) {
    hexbasenl(fp, ptr, len);
    fprintf(fp, "%s\n", "");
}

/*
#include <string.h>
int main() {
    hexbase( stdout, "hello world", strlen("hello world") );                   // --> hello world
    hexbase( stdout, "hello world \x1\x2\x3\xff", strlen("hello world") + 5 ); // --> hello world ~010203ff~
    hexbase( stdout, "hello~world", strlen("hello~world") );                   // --> hello~~world
    hexbase( stdout, "\xa1\2\3", 3 );                                          // --> ~a10203~
    hexbase( stdout, "\1\2\3hello world", 3 + strlen("hello world") );         // --> ~01020368656c6c6f20776f726c64~
    hexbase( stdout, "\1\2\3hello world\xf0", 4 + strlen("hello world") );     // --> ~01020368656c6c6f20776f726c64f0~
}
*/
