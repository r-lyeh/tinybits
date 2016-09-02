// Tiny endianness. rlyeh, public domain | wtrmrkrlyeh
#pragma once
#include <stdint.h>

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\1" == 1)
static uint16_t swap16( uint16_t x ) { return (x << 8) | (x >> 8); }
static uint32_t swap32( uint32_t x ) { return (x << 24) | (x >> 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00); }
static uint64_t swap64( uint64_t x ) { return (x << 56) | (x >> 56) | ((x & 0xff00) << 40) | ((x >> 40) & 0xff00) | ((x & 0xff0000) << 24) | ((x >> 24) & 0xff0000) | ((x & 0xff000000) << 8) | ((x >> 8) & 0xff000000); }
static uint16_t tobe16( uint16_t x ) { return IS_BIG_ENDIAN ? x : swap16(x); }
static uint32_t tobe32( uint32_t x ) { return IS_BIG_ENDIAN ? x : swap32(x); }
static uint64_t tobe64( uint64_t x ) { return IS_BIG_ENDIAN ? x : swap64(x); }
static uint16_t tole16( uint16_t x ) { return IS_BIG_ENDIAN ? swap16(x) : x; }
static uint32_t tole32( uint32_t x ) { return IS_BIG_ENDIAN ? swap32(x) : x; }
static uint64_t tole64( uint64_t x ) { return IS_BIG_ENDIAN ? swap64(x) : x; }

#ifdef TINYENDIAN_MAIN
#include <stdio.h>
void TINYENDIAN_MAIN() {
    printf("%x\n", swap32(0x12345678) );
}
#endif
