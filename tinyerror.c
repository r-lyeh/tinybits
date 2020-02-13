// simple error handling api. non-intrusive version.
// - rlyeh, public domain.
// 
// Errors automatically printed in debug builds.
// Usage: use return OK(retvalue) or return ERROR(retvalue, "error"...); as desired.
// Usage: to check for errors: if(ERROR) { /* do something here */ }
// Good habit: system errorcode first, then human explanation when reporting errors. Ie, ERROR(rc, "404 File not found %s\n", file);

#pragma once
#include <stdio.h>

#  if !defined ERROR_LOG && defined NDEBUG
#define ERROR_LOG(...)       (void)0
#elif !defined ERROR_LOG
#define ERROR_LOG(...)       (fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\n"))
#endif

#define OK(retval)         (ERROR = (char*)0, (retval))
#define ERROR(retval, ...) (ERROR_LOG("" __VA_ARGS__), ERROR = (char*)"Error: " #__VA_ARGS__ " (@" __FUNCTION__ " " __FILE__ ":" ERR0R(__LINE__) ") ", (retval))
#define ERR0R(rc)           ERRoR(rc)
#define ERRoR(rc)           #rc

#  ifdef __GNUC__
#define __FUNCTION__ ""
static __thread char* ERROR = 0;
#else // _MSC_VER
static __declspec(thread) char* ERROR = 0;
#endif

#if 0 // demo
int derefence(int *ptr) {
    if(ptr) return OK(*ptr);
    return ERROR(0, "404: Cannot deference pointer [%p]", ptr); // errorcode + variable message (ideal)
    return ERROR(0, "Cannot deference pointer [%p]", ptr);      // dynamic message (no errorcode)
    return ERROR(0, "Cannot deference pointer");                // fixed message (no errorcode)
    return ERROR(0);                                            // (no message) (no errorcode)
}
int main(int arg, char **argv) {
    int a = derefence(&arg); // pass
    int b = derefence(NULL); // fail
    if( ERROR ) { /* do something here */ }
}
#endif
