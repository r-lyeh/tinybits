// tiny benchmarks. OpenMP required.
// - rlyeh, public domain | wtrmrkrlyeh
#pragma once
#include <omp.h>
#include <stdio.h>
struct bench {
    double line, time;
    operator bool() const { return true; }
    ~bench() { printf("L%d %gms\n", (int)line, (omp_get_wtime() - time) * 1000); }
};
#define bench if( const bench x = { __LINE__, omp_get_wtime() } ) 

#ifdef TINYBENCHMARK_MAIN
void TINYBENCHMARK_MAIN() {
   bench {
        for( int i = 0; i < 100000000; ++i );
        puts("hello stdio");
   }
}
#endif
