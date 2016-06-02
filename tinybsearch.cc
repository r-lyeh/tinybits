// Tiny binary search (dichotomic): container must be sorted && supporting sequential access.
// - rlyeh, public domain

template<typename container, typename T>
unsigned bsearch( const T &x, const container &v ) {
    int min = 0, max = int(v.size());
    while( min <= max ) {
        int mid = min + ( max - min ) / 2;
        /**/ if( x == v[mid] ) return mid;
        else if( x  < v[mid] ) max = mid - 1;
        else min = mid + 1;
    }
    return ~0u;
}

#include <cassert>
#include <vector>
int main() {
                    // @ [0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12]
    std::vector<int> v {  0,  1,  2,  3,  5,  6,  7, 10, 11, 12,  13,  15,  16 };

    assert( bsearch( 0, v) ==   0 ); // @ [ 0]
    assert( bsearch(10, v) ==   7 ); // @ [ 7]
    assert( bsearch(11, v) ==   8 ); // @ [ 8]
    assert( bsearch(15, v) ==  11 ); // @ [11]
    assert( bsearch(16, v) ==  12 ); // @ [12]
    assert( bsearch(-3, v) == ~0u ); // not found
    assert( bsearch(18, v) == ~0u ); // not found
    assert( bsearch( 8, v) == ~0u ); // not found
    assert( bsearch( 9, v) == ~0u ); // not found

    for( const auto &i : v ) {
        assert( v[bsearch(i,v)] == i );
    }
}
