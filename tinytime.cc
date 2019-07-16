// Tiny timing utilities. rlyeh, public domain | wtrmrkrlyeh
#include <thread>
#include <chrono>
#if !defined(TIMING_USE_OMP) && ( defined(USE_OMP) || defined(_MSC_VER) /*|| defined(__ANDROID_API__)*/ )
#   define TIMING_USE_OMP
#   include <omp.h>
#endif
double now() {
#   ifdef TIMING_USE_OMP
    static auto const epoch = omp_get_wtime();
    return omp_get_wtime() - epoch;
#   else
    static auto const epoch = std::chrono::steady_clock::now(); // milli ms > micro us > nano ns
    return std::chrono::duration_cast< std::chrono::microseconds >( std::chrono::steady_clock::now() - epoch ).count() / 1000000.0;
#   endif
}
double bench( void (*fn)() ) {
    double took = -now();
    return ( fn(), took + now() );
}
void sleep( double secs ) {
    std::chrono::microseconds duration( (int)(secs * 1000000) );
    std::this_thread::sleep_for( duration );
}

/*
#include <stdio.h>
int main() {
    double t0 = now();
    printf("%g s.\n", bench( []{ sleep(0.1234); } ) );
    double t1 = now();
    printf("%g s.\n", t1 - t0 );
}
*/
