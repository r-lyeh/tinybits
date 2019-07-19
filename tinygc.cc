// tiny garbage collector (<100 LOCs). Genius code tricks by @orangeduck (see: https://github.com/orangeduck/tgc README)
// - rlyeh, public domain.

void         gc_init(void *argc);           // pointer to argc (from main)
void         gc_run(void);                  // mark & sweep
void         gc_stop(void);                 // sweep

void*        gc_malloc(int sz);             // allocator
void         gc_free(void *ptr);            // optional
char*        gc_strdup(const char *str);    // util

#define GC(type) type const // macro to raise compile-time errors when doing pointer arithmetic (++,--,+=...) on GC pointers

// ---

#include <setjmp.h> // setjmp, jmp_buf
#include <stdlib.h> // realloc
#include <string.h> // memcpy, strlen
#include <stdio.h>  // printf
#include <set>      // std::set<>

#ifndef GC_REALLOC
#define GC_REALLOC realloc
#endif

static void *gc_top = 0, *gc_min = 0, *gc_max = (void*)~0ull; // ~0ull == UINTPTR_MAX
static std::set<void*> spawned, inuse;

static void gc_mark_stack(void) {
    void *bot = gc_top, *top = &bot, *last = 0;

    for( void *p = bot<top?bot:top, *e = bot<top?top:bot; p <= e; p = (char*)p + sizeof(void*) ) {
        void *ptr = *((void**)p);

        if( ptr == last  ) continue; // already checked
        if( ptr < gc_min ) continue; // out of spawned bounds. also, p == NULL included here
        if( ptr > gc_max ) continue; // out of spawned bounds.

        if( spawned.find(ptr) == spawned.end() ) continue; // not in gc pool
        inuse.insert(last = ptr);
    }
}
static void gc_mark() { // mark reachable stack pointers
    jmp_buf env = {0};
    void (*volatile check)(void) = gc_mark_stack;
    setjmp(env);
    check();
}
static void gc_sweep() { // sweep unreachable stack pointers
    gc_min = (void*)~0ull, gc_max = 0; // ~0ull == UINTPTR_MAX

    int count = 0;
    for( auto iter = spawned.begin(); iter != spawned.end(); ) {
        void *ptr = *iter;
        if( ptr > gc_max ) gc_max = ptr;
        if( ptr < gc_min ) gc_min = ptr;

        bool used = inuse.find(ptr) != inuse.end();
        if( !used ) {
            GC_REALLOC(ptr, 0); //free
            iter = spawned.erase( iter );
            ++count;
        } else {
            ++iter;
        }
    }
    inuse.clear();

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
    if( ptr ) spawned.insert(ptr);
    return ptr;
}

void gc_free( void *ptr ) {
    if( ptr ) spawned.erase(ptr);
    if( ptr ) GC_REALLOC(ptr, 0); // free
}

char *gc_strdup( const char *s ) {
    int bytes = (int)strlen(s)+1;
    return (char *)memcpy(gc_malloc(bytes), s, bytes);
}

/*
#include <time.h>
#define benchmark(...) for(clock_t beg=clock(), end=beg-beg; !end; printf("" __VA_ARGS__), printf("%5.2fs\n", (end=clock()-beg) / (double)CLOCKS_PER_SEC))

void bench() {
    enum { FRAMES = 3, COUNT = 1000000 };
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
*/
