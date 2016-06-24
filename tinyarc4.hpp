// tinyARC4, ARC4 stream cypher. based on code by Mike Shaffer.
// - rlyeh, public domain ~~ listening to Black Belt - Leeds

#pragma once
#include <string>

std::string tinyARC4( const std::string &text, const std::string &passkey ) {
    int sbox[256], key[256];
    std::string output;
    size_t plen = passkey.size(), tlen = text.size();
    if( plen ) {
        output.resize( text.size() );
        for( size_t a = 0; a < 256; a++ ) {
            key[ a ] = passkey[ a % plen ];
            sbox[ a ] = a;
        }
        for( size_t a = 0, b = 0; a < 256; a++ ) {
            b = (b + sbox[ a ] + key[ a ]) % 256;
            int swap = sbox[ a ]; sbox[ a ] = sbox[ b ]; sbox[ b ] = swap;
        }
        for( size_t a = 0, i = 0, j = 0, k; a < tlen; a++ ) {
            i = (i + 1) % 256;
            j = (j + sbox[ i ]) % 256;
            int swap = sbox[ i ]; sbox[ i ] = sbox[ j ]; sbox[ j ] = swap;
            k = sbox[(sbox[ i ] + sbox[j]) % 256];
            output[ a ] = text[ a ] ^ k;
        }
    } else output = text;
    return output;
}

#ifdef TINYARC4_MAIN
#include <cassert>
#include <iostream>
int TINYARC4_MAIN( int argc, const char **argv ) {
    // sample
    std::string encrypted = tinyARC4( "Hello world.", "my-password" );
    std::string decrypted = tinyARC4( encrypted, "my-password" );

    std::cout << "ARC4 Encrypted text: " << encrypted << std::endl;
    std::cout << "ARC4 Decrypted text: " << decrypted << std::endl;

    // tests
    assert( tinyARC4("hello world", "my key") != "hello world" );
    assert( tinyARC4(tinyARC4("hello world", "my key"), "my key") == "hello world" );
}
#endif
