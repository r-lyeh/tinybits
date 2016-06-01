// Tiny binary search (dichotomic): container must be sorted && supporting sequential access.
// - rlyeh, public domain

template<typename container, typename T>
unsigned find( const T &x, const container &v ) {
    int min = 0, max = int(v.size());
    while( min <= max ) {
        int mid = min + ( max - min ) / 2;
        /**/ if( x == v[mid] ) return mid;
        else if( x  < v[mid] ) max = mid - 1;
        else min = mid + 1;
    }
    return ~0u;
}

#if 0 
// C version:
#include <string.h>
unsigned find( const char *key, const char *array, int numelems ) {
    int min = 0, max = numelems;
    while( min <= max ) {
        int mid = min + ( max - min ) / 2;
        int search = strcmp(key, array[mid]);
        /**/ if( 0 ==search ) return mid;
        else if( search < 0 ) max = mid - 1;
        else min = mid + 1;
    }
    return ~0u;
}
#endif

#include <cassert>
#include <vector>
int main() {
                    // @ [0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12]
    std::vector<int> v {  0,  1,  2,  3,  5,  6,  7, 10, 11, 12,  13,  15,  16 };

    assert( find( 0, v) ==   0 ); // @ [ 0]
    assert( find(10, v) ==   7 ); // @ [ 7]
    assert( find(11, v) ==   8 ); // @ [ 8]
    assert( find(15, v) ==  11 ); // @ [11]
    assert( find(16, v) ==  12 ); // @ [12]
    assert( find(-3, v) == ~0u ); // not found
    assert( find(18, v) == ~0u ); // not found
    assert( find( 8, v) == ~0u ); // not found
    assert( find( 9, v) == ~0u ); // not found

    for( const auto &i : v ) {
        assert( v[find(i,v)] == i );
    }
}
