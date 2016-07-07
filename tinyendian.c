// Tiny endianness. rlyeh, public domain | wtrmrkrlyeh
#include <stdint.h>

#if __STDC_VERSION__ >= 199901L
#define IS_BIG_ENDIAN (!*(unsigned char *)&(uint16_t){1})
#else
#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)
#endif

uint16_t swap16( uint16_t x ) {
    return ( x << 8 ) | ( x >> 8 );
}
uint32_t swap32( uint32_t x ) {
    return ( x << 24 ) | (( x << 8 ) & 0xff0000) | (( x >> 8 ) & 0xff00) | ( x >> 24 );
}
uint16_t tobe16( uint16_t x ) {
    return IS_BIG_ENDIAN ? x : swap16(x);
}
uint32_t tobe32( uint32_t x ) {
    return IS_BIG_ENDIAN ? x : swap32(x);
}
uint16_t tole16( uint16_t x ) {
    return IS_BIG_ENDIAN ? swap16(x) : x;
}
uint32_t tole32( uint32_t x ) {
    return IS_BIG_ENDIAN ? swap32(x) : x;
}

#include <stdio.h>
int main() {
    printf("%d\n", tole32(0x12345678) );
}
