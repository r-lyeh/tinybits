// Tiny binary search (dichotomic): array must be sorted && supporting sequential access.
// - rlyeh, public domain | wtrmrkrlyeh

#include <string.h>

unsigned bsearchint( const int *array, int numelems, int key ) {
    int min = 0, max = numelems;
    while( min <= max ) {
        int mid = min + ( max - min ) / 2;
        /**/ if( key == array[mid] ) return mid;
        else if( key <  array[mid] ) max = mid - 1;
        else min = mid + 1;
    }
    return ~0u;
}
unsigned bsearchsz( const size_t *array, int numelems, size_t key ) {
    int min = 0, max = numelems;
    while( min <= max ) {
        int mid = min + ( max - min ) / 2;
        /**/ if( key == array[mid] ) return mid;
        else if( key <  array[mid] ) max = mid - 1;
        else min = mid + 1;
    }
    return ~0u;
}
unsigned bsearchstr( const char **array, int numelems, const char *key ) {
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

#include <assert.h>

int main() {
                      // @ [0]    [1]            [2]        [3]
    const char *dict[] = { "abc", "abracadabra", "ale hop", "all your base"};

    assert( bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), "abc") == 0 );           // @ [0]
    assert( bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), "abracadabra") == 1 );   // @ [1]
    assert( bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), "ale hop") ==   2 );     // @ [2]
    assert( bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), "all your base") == 3 ); // @ [3]

    assert( bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), "are belong to us") == ~0u ); // not found
    assert( bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), "move") == ~0u ); // not found
    assert( bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), "every") == ~0u ); // not found
    assert( bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), "zig") == ~0u ); // not found
    assert( bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), "") == ~0u ); // not found

    for( int i = 0; i < sizeof(dict) / sizeof(dict[0]); ++i ) {
        assert( i == bsearchstr(dict, sizeof(dict) / sizeof(dict[0]), dict[i]) );
    }
}
