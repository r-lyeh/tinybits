// tiny garbage collector (<100 LOCs). Genius code tricks by @orangeduck (see: https://github.com/orangeduck/tgc README)
// - rlyeh, public domain.

void         gc_init(void *argc);           // pointer to argc (from main)
void         gc_run(void);                  // mark & sweep
void         gc_stop(void);                 // sweep

void*        gc_malloc(int sz);             // allocator
void         gc_free(void *ptr);            // optional
char*        gc_strdup(const char *str);    // util

// helper macro for sane stack backtracking of our allocated pointers: raises errors on pointer arithmetics.
#define GC(type) type const 

// ---

#include <setjmp.h> // setjmp, jmp_buf
#include <stdlib.h> // realloc
#include <string.h> // memcpy, strlen
#include <stdio.h>  // printf
#include <stdint.h> // uintptr_t, UINTPTR_MAX
#include <set>      // std::set<>

#ifndef GC_REALLOC
#define GC_REALLOC realloc
#endif

static void *gc_top = 0, *gc_min = 0, *gc_max = (void*)UINTPTR_MAX;
static std::set<void*> gc_spawned, gc_inuse;

static void gc_mark_stack(void) {
    void *bot = gc_top, *top = &bot, *last = 0;

    for( void *p = bot<top?bot:top, *e = bot<top?top:bot; p < e; p = (char*)p + sizeof(void*) ) {
        void *ptr = *((void**)p);

        if( ptr == last  ) continue; // already checked
        if( ptr < gc_min ) continue; // out of gc_spawned bounds. also, ptr == NULL check included here
        if( ptr > gc_max ) continue; // out of gc_spawned bounds.
        if( (uintptr_t)ptr & 0x7 ) continue; // 64-bit unaligned (not a pointer).

        if( gc_spawned.find(ptr) == gc_spawned.end() ) continue; // not in gc pool
        gc_inuse.insert(last = ptr);
    }
}
static void gc_mark() { // mark reachable stack pointers
    jmp_buf env = {0};
    void (*volatile check)(void) = gc_mark_stack;
    setjmp(env);
    check();
}
static void gc_sweep() { // sweep unreachable stack pointers
    gc_min = (void*)UINTPTR_MAX, gc_max = 0;

    int count = 0;
    for( auto iter = gc_spawned.begin(); iter != gc_spawned.end(); ) {
        void *ptr = *iter; // khash: void **p = gc_spawned.exist(iter); if (!p) { ++iter;  continue; } void *ptr = *p;

        if( ptr > gc_max ) gc_max = ptr;
        if( ptr < gc_min ) gc_min = ptr;

        bool used = gc_inuse.find(ptr) != gc_inuse.end();
        if( !used ) {
            GC_REALLOC(ptr, 0); //free
            iter = gc_spawned.erase( iter );
            ++count;
        } else {
            ++iter;
        }
    }
    gc_inuse.clear();

    if( count ) printf("gc: %9d objects collected\n", count);
}

void gc_init(void *argc) {
    gc_top = argc;
}
void gc_run() {
    gc_mark();
    gc_sweep();
}
void gc_stop() {
    gc_sweep();
}

void *gc_malloc( int sz ) {
    void *ptr = GC_REALLOC(0, sz); // malloc
    if( ptr ) gc_spawned.insert(ptr);
    return ptr;
}
void gc_free( void *ptr ) {
    if( ptr ) gc_spawned.erase(ptr);
    if( ptr ) GC_REALLOC(ptr, 0); // free
}
char *gc_strdup( const char *s ) {
    int bytes = (int)strlen(s)+1;
    return (char *)memcpy(gc_malloc(bytes), s, bytes);
}

/*
#ifdef _MSC_VER
#include <omp.h>
#define benchmark(...) for(double end=-omp_get_wtime(); end<=0; printf("" __VA_ARGS__), printf(" %5.2fs\n", end+=omp_get_wtime()))
#else
#include <time.h>
#define benchmark(...) for(clock_t beg=clock(), end=beg-beg; !end; printf("" __VA_ARGS__), printf(" %5.2fs\n", (end=clock()-beg) / (double)CLOCKS_PER_SEC))
#endif

void bench() {
    enum { FRAMES = 30, COUNT = 1000000 };
    benchmark("%2.1fM allocs/frees", FRAMES * COUNT * 2 / 1000000.0) {
        for( int frame = 0; frame < FRAMES; ++frame ) {
            for( int n = 0; n < COUNT; ++n ) {
                void *p = gc_strdup("hello world"); (void)p;
            }
        }
        gc_run();
    }
}

void demo() {
    GC(void*) memory = gc_malloc(1024); (void)memory;     // will be collected
    GC(char*) string = gc_strdup("hello world");          // will be collected
    GC(void*) proper = gc_malloc(1); gc_free(proper);     // wont be collected (explicit free)
    GC(char*) x = gc_strdup("Hi"); x[0] |= 32;            // *x++ |= 32 would raise compile-time error (thanks to GC() macro).
    gc_run();

    puts(string);
    gc_run();
}

int main(int argc, char **argv) {
    gc_init(&argc); (void)argv;

    demo();
    bench();

    gc_stop();
}

// benchmark results, 60 million ops:
// 11.02s spp::sparse_hash_set<T>          #include "sparsepp/spp.h"
// 13.71s std::set<T>                      #include <set>
// 21.26s phmap::flat_hash_set<T>          #include "parallel-hashmap/phmap.h"
// 21.57s phmap::parallel_flat_hash_set<T> #include "parallel-hashmap/phmap.h"
// 22.12s klib::KHash<T, std::hash<T>>     #include "khash.hpp"
// 29.01s std::unordered_set<T>            #include <unordered_set>
// 41.38s phmap::parallel_node_hash_set<T> #include "parallel-hashmap/phmap.h"
// 42.48s phmap::node_hash_set<T>          #include "parallel-hashmap/phmap.h"
*/
