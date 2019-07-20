// tiny garbage collector (<100 LOCs). Genius code tricks by @orangeduck (see: https://github.com/orangeduck/tgc README)
// - rlyeh, public domain.

void  gc_init(void *argc, int initial_mbytes_reserved); // pointer to argc (from main), and initial MiB reserved
void  gc_run(void);                                     // mark & sweep
void  gc_stop(void);                                    // sweep

void* gc_malloc(int sz);                                // allocator
char* gc_strdup(const char *str);                       // util

// macro that forbids pointer arithmetic (enables fixed pointer addresses)
#define GC(type) type const 

// ---

#include <setjmp.h> // setjmp, jmp_buf
#include <stdlib.h> // realloc
#include <string.h> // memcpy, strlen
#include <stdio.h>  // printf
#include <stdint.h> // uintptr_t, UINTPTR_MAX
#include <set>      // std::set<>
#include <vector>   // std::vector<>

#ifndef GC_REALLOC
#define GC_REALLOC realloc
#endif

static std::set<void*> gc_inuse;
static std::vector<void*> gc_spawned;
static void *gc_top = 0, *gc_min = 0, *gc_max = (void*)UINTPTR_MAX;

static void gc_mark_stack(void) {
    void *bot = gc_top, *top = &bot, *last = 0;

    for( void *p = bot<top?bot:top, *e = bot<top?top:bot; p < e; p = (char*)p + sizeof(void*) ) {
        void *ptr = *((void**)p);

        if( ptr == last  ) continue; // already checked
        if( ptr < gc_min ) continue; // out of gc_spawned bounds. also, nullptr check included here
        if( ptr > gc_max ) continue; // out of gc_spawned bounds.
        if( (uintptr_t)ptr & 0x7 ) continue; // 64-bit unaligned (not a pointer).

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

    size_t back = gc_spawned.size();
    for( size_t i = 0; i < back; ++i) {
        void *ptr = gc_spawned[i];

        if( ptr > gc_max ) gc_max = ptr;
        if( ptr < gc_min ) gc_min = ptr;

        bool used = gc_inuse.find(ptr) != gc_inuse.end();
        if( !used ) {
            GC_REALLOC(gc_spawned[i], 0); //free

            void *swap = gc_spawned[--back]; // vector erase
            gc_spawned[back] = gc_spawned[i];
            gc_spawned[i--] = swap;
        }
    }

    size_t collected = gc_spawned.size() - back;
    if( collected ) printf("gc: %9d objects collected\n", (int)collected);

    gc_spawned.resize( back );
    gc_inuse.clear();
}

void gc_init(void *argc, int MiB) {
    gc_top = argc;
    gc_spawned.reserve((MiB > 0) * MiB * 1024 * 1024 / sizeof(void*));
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
    if( ptr ) gc_spawned.push_back(ptr);
    return ptr;
}
char *gc_strdup( const char *s ) {
    int bytes = (int)strlen(s)+1;
    return (char *)memcpy(gc_malloc(bytes), s, bytes);
}

/*
#include <time.h>
#define benchmark(t,...) for(clock_t beg=clock(), end=beg-beg; !end; printf("" __VA_ARGS__), printf(" %5.2fs\n", t=((end=clock()-beg) / (double)CLOCKS_PER_SEC)))

void bench() {
    enum { FRAMES = 30, COUNT = 1000000 };

    double baseline;
    benchmark(baseline, "%2.1fM allocs+frees (baseline; regular malloc)", FRAMES * COUNT * 2 / 1000000.0) {
        for( int frame = 0; frame < FRAMES; ++frame ) {
            for( int n = 0; n < COUNT; ++n ) {
                free( malloc(16) );
            }
        }
    }

    double gctime;
    benchmark(gctime, "%2.1fM allocs+frees (gc)", FRAMES * COUNT * 2 / 1000000.0) {
        for( int frame = 0; frame < FRAMES; ++frame ) {
            for( int n = 0; n < COUNT; ++n ) {
                (void)gc_malloc(16);
            }
        }
        gc_run();
    }

         if(baseline<=gctime) printf("gc is x%.2f times slower\n", gctime/baseline);
    else if(baseline >gctime) printf("gc is x%.2f times faster!\n", baseline/gctime);
}

void demo() {
    GC(void*) memory = gc_malloc(1024); (void)memory;     // will be collected
    GC(char*) string = gc_strdup("hello world");          // will be collected
    GC(char*) x = gc_strdup("Hi"); x[0] |= 32;            // will be collected. note: indexing is ok; pointer arithmetic is forbidden.
    gc_run();

    puts(string);
    gc_run();
}

int main(int argc, char **argv) {
    gc_init(&argc, 256); (void)argv;

    demo();
    bench();

    gc_stop();
}
*/
