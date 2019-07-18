// tiny garbage collector (<100 LOCs)
// - rlyeh, public domain.
//
// implementation details:
//   mark: list of pointers that have been allocated by app.
//   sweep: list of pointers that are still in use.
//   gc: remove all pointers present in [mark] list that do not exist in [sweep] list.
//   thread policy: one gc state per thread. do not mix pointers from different threads freely.

// basics
void* gc_malloc( int sz );    // allocate memory
void  gc_free( void *ptr );   // optional free
void  gc_use( void **ptr );   // call before use
void  gc_unuse( void **ptr ); // call after use
void  gc();                   // garbage collect. usually done at vsync every N seconds.

// utils
void *gc_calloc( int count, int size );
void *gc_strcpy( const char *buf );

// ----------------------------------------------------------------------------

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <set> // see also: <unordered_set>

#define gc__assert(ptr, expected) assert( (((uintptr_t)ptr) & 1) == expected ) // unused addr bit check
#define gc__swap(ptr)            (void *)(~((uintptr_t)ptr))                  // swap bits

#if defined _MSC_VER && !defined __thread
#define __thread __declspec(thread)
#endif
static __thread std::set<void*> mark, sweep;

void *gc_malloc( int sz ) {
    void *ptr = malloc(sz);
    gc__assert(ptr, 0);
    mark.insert(ptr);
    return gc__swap(ptr);
}

void gc_free( void *ptr ) { // optional
    gc__assert(ptr, 1);
    ptr = gc__swap(ptr);
    free(ptr);
    mark.erase(ptr);
    sweep.erase(ptr);
}

void gc_use( void **ptr ) {
    gc__assert(*ptr, 1);
    *ptr = gc__swap(*ptr);
    sweep.insert(*ptr);
}

void gc_unuse( void **ptr ) {
    gc__assert(*ptr, 0);
    sweep.erase(*ptr);
    *ptr = gc__swap(*ptr);
}

void gc() {
    int collected = 0;
    for( auto iter = mark.begin(); iter != mark.end(); ) {
        if( sweep.find(*iter) == sweep.end() ) {
            void *ptr = *iter;
            free(ptr);
            ++collected;

            iter = mark.erase(iter);
        } else {
            ++iter;
        }
    }
    printf("gc: %d pointers collected\n---\n", collected);
}

void *gc_calloc( int count, int size ) {
    void *p = gc_malloc( count * size );
    void *t = gc__swap( p );
    memset(t, 0, count * size);
    return p;
}

void *gc_strcpy( const char *buf ) {
    int l = strlen(buf);
    void *p = gc_malloc( l + 1 );
    void *t = gc__swap(p);
    memcpy(t, buf, l + 1 );
    return p;
}

/*
int main() {
    if( !puts("#1: create->collect test") ) {
        void *p = gc_strcpy("hello world");
        void *q = gc_strcpy("hello world");

        gc();
    }

    if( !puts("#2: create->free->collect test") ) {
        void *p = gc_strcpy("hello world");
        void *q = gc_strcpy("hello world");

        gc_free(p);
        gc();
    }

    if( !puts("#3: create->use->unuse->collect test") ) {
        void *p = gc_strcpy("hello world");
        void *q = gc_strcpy("hello world");

        gc_use(&p);
        puts((char*)p);
        gc_unuse(&p);

        gc();
    }

    if( !puts("#4: create->use->collect->unuse->collect test") ) {
        void *p = gc_strcpy("hello world");
        void *q = gc_strcpy("hello world");

        gc_use(&p);
        puts((char*)p);
        gc();
        gc_unuse(&p);

        gc();
    }
}
*/
