// old assert() macro with new tricks.
// - rlyeh, public domain.
//
// - [x] log failures always. break debugger only once per failure.
// - [x] enabled always. even in optimized builds. unless ASSERT_LEVEL is 0.
// - [x] allow messages. ie, assert(my_var != 5, "failed my_var!=%d", my_var).
// - [x] break debugger only if present (hassle developers; dont hassle users).
// - [x] overridable logger & debugger break. tip: may define ASSERT_BREAK as abort(), or leave it blank as well.
// - [x] filter out by configurable range (to speed up things when having thousand of expensive asserts). see ASSERT_LEVEL.

// configuration: percentage of random checked asserts: [0%(none)..50%(half)..100%(all)]. default: 100%
#ifndef ASSERT_LEVEL
#define ASSERT_LEVEL 100 
#endif

// impl below ------------------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#undef assert

// default logger: dump to stderr
#ifndef ASSERT_LOG
#define ASSERT_LOG(x) do { fprintf(stderr, "%s\n", (x)); } while(0)
#endif

// default break behavior: break if debugged.
#  if !defined ASSERT_BREAK && defined _WIN32
#define ASSERT_BREAK() do{ if(IsDebuggerPresent()) __debugbreak(); } while(0)
#include <winsock2.h> // needed for __cplusplus
#elif !defined ASSERT_BREAK // && defined __unix__
#define ASSERT_BREAK() do { signal(SIGTRAP, break_handler_); raise(SIGTRAP); } while(0)
#include <signal.h>
static void break_handler_(int signum) { signal(SIGTRAP, SIG_DFL); }
#endif

#if (ASSERT_LEVEL <= 0) || defined SHIPPING
#define assert(EXPR, ...) (void)0
#else
#define assert(EXPR, ...) do { \
    static int64_t maybe = -1; if(maybe < 0) { \
            /* original splitmix64 by Sebastiano Vigna (CC0)*/ \
            uint64_t z = (__LINE__ + __COUNTER__ + UINT64_C(0x9E3779B97F4A7C15)); \
            z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9); \
            maybe = (unsigned)((z ^ (z >> 31)) % 100) < ASSERT_LEVEL; } \
        if( maybe ) { \
            int msvc_trick[] = {0,}; \
            if( !(EXPR) ) { \
                char text[4096], *ptr = text; \
                ptr += sprintf( ptr, "!" __VA_ARGS__ ); \
                ptr += text[1] ? 0 : sprintf( ptr, "Assertion failed: %s", #EXPR ); \
                ptr += sprintf( ptr, " (unexpected) %s:%d", __FILE__, __LINE__); \
                ASSERT_LOG((1+text)); \
                msvc_trick[0]++; /* fool clang -Wunused-variable */ \
                static int once = 1; for(;once;once=0) ASSERT_BREAK(); \
            } \
        } \
    } while(0)
#endif

#if 0 // demo
int main() {
    for( int i = 0; i < 3; ++i ) {
        assert(i < 2);
        assert(i < 2, "Error! %d should be smaller than %d", i, 2);
    }
    puts("Program continues over here unless debugger is attached...");
}
#endif
