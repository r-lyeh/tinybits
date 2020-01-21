// C string library
// - rlyeh, public domain

// temporary strings api (stack)
char* strtmp(const char *fmt, ...);
int stristmp(const char *s);

// allocated strings api (heap)
#define strnew(fmt, ...) strdup(strtmp(fmt,__VA_ARGS__))
#define strdel(s)        ((stristmp(s) ? (void)0 : free(s)), (s)=0)

// implementation --------------------------------------------------------------

#if defined _MSC_VER && !defined __thread
#define __thread __declspec(thread)
#endif

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int stristmp(const char *s) { // is_va()
    return (1&(uintptr_t)s) && s[-1] == 0;
}
char* strtmp(const char *fmt, ...) { // va()
    va_list vl;
    va_start(vl, fmt);
    int sz = vsnprintf( 0, 0, fmt, vl ) + 1;
    va_end(vl);

    int reqlen = sz + 1; // 1 for even padding

    char* ptr;
    enum { STACK_ALLOC = 16384 };
    if( reqlen < STACK_ALLOC ) { // fit stack?
        static __thread char buf[STACK_ALLOC+1];
        static __thread int cur = 1, len = STACK_ALLOC;
        ptr = buf + ((cur+reqlen) > len ? cur = 1 : (cur += reqlen) - reqlen);
    } else { // else use heap (@fixme: single memleak per invoking thread)
        static __thread char *buf = 0;
        static __thread int cur = 1, len = -STACK_ALLOC;
        if( reqlen >= len ) buf = realloc(buf, len = abs(len) * 1.75 + reqlen);
        ptr = buf + ((cur+reqlen) > len ? cur = 1 : (cur += reqlen) - reqlen);
    }

    ptr += !(1&(uintptr_t)ptr); // align to even address only when already odd
    ptr[-1] = 0; // add header
    assert(stristmp(ptr));

    va_start(vl,fmt);
    vsnprintf( ptr, sz, fmt, vl );
    va_end(vl);

    return (char *)ptr;
}

/*
int main() {
    // creation
    char *x = strtmp("hello %d", 123); puts(x);
    char *y = strnew("hello %d", 123); puts(y);
    assert(strcmp(x,y)==0);

    // destruction
    // strdel(x); assert(x == 0); // optional free, since x is a temporary string (strtmp)
    strdel(y); assert(y == 0);    // required free, since y was explicitly allocated (with strnew)

    // test concat
    char *z = strtmp("%d",6); z = strtmp("%s%s%s",z,z,z); assert( 0 == strcmp(z,"666") );

    // test memory is never exhausted
    for(int i = 0; i < 10000; ++i) assert(strtmp("hello %d",123));

    // test asserts are enabled
    assert(~puts("Ok"));
}
*/
