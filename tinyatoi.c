// Tiny atoi. rlyeh, public domain

int atoi( const char *cstr ) {
    int integer = 0, neg = 1;
    if ( cstr ) {
        for( ; *cstr == '-' ; cstr++ ) {
            neg *= -1;
        }
        for( ; (*cstr >= '0') && (*cstr <= '9'); cstr++ ) {
            integer = (integer * 10) + *cstr - '0';
        }
    }
    return neg * integer;
}

#include <stdio.h>
int main() {
    printf("%d\n", atoi("01230"));
    printf("%d\n", atoi("-01230"));
    printf("%d\n", atoi("--01230"));
    printf("%d\n", atoi("---01230"));
}
