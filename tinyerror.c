// simple error handling api
// - rlyeh, public domain.

#define error(rc, ...)  (error = (rc) ? "Error " err0r(rc) ": " __VA_ARGS__ : 0)
#define errorcode       (error ? atoi(error+6) : 0)

// utils

#define err0r(rc)       err1r(rc)
#define err1r(rc)       #rc

#ifndef _MSC_VER
static __thread char* error = 0;
#else
static __declspec(thread) char* error = 0;
#endif


// -------------------

#include <stdio.h>

FILE *open_file(const char *file) {
    error(0);
    FILE *fp = fopen(file, "rb");
    if( !fp ) error(404, "Cannot open file");
    return fp;
}

int main(int argc, char **argv ) {
    FILE *fp = open_file(__FILE__);

    if( !error ) {
        printf("ok\n");
    }
    if( error ) {
        printf("%s\n", error);
    }
    if( errorcode ) {
        printf("%d\n", errorcode);
    }
}
