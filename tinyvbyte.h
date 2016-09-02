// tiny variable byte length encoder/decoder (vbyte)
// - rlyeh, public domain | wtrmrkrlyeh
#pragma once
#include <stdint.h>

enum { VBYTE_MIN_REQ_BYTES = 1, VBYTE_MAX_REQ_BYTES = 10 };

static uint64_t vbuencode( uint8_t *buffer, uint64_t value ) {
    /* 7-bit packing. MSB terminates stream */
    const uint8_t *buffer0 = buffer;
    do {
        *buffer++ = (uint8_t)( 0x80 | (value & 0x7f) );
        value >>= 7;
    } while( value > 0 );
    *(buffer-1) ^= 0x80;
    return buffer - buffer0;
}
static uint64_t vbudecode( uint64_t *value, const uint8_t *buffer ) {
    /* 7-bit unpacking. MSB terminates stream */
    const uint8_t *buffer0 = buffer;
    uint64_t out = 0, j = -7;
    do {
        out |= (( ((uint64_t)(*buffer)) & 0x7f) << (j += 7) );
    } while( ((uint64_t)(*buffer++)) & 0x80 );
    *value = out;
    return buffer - buffer0;
}

static uint64_t vbiencode( uint8_t *buffer, int64_t value ) {
    /* convert sign|magnitude to magnitude|sign */
    uint64_t nv = (uint64_t)((value >> 63) ^ (value << 1));
    /* encode unsigned */
    return vbuencode( buffer, nv );
}
static uint64_t vbidecode( int64_t *value, const uint8_t *buffer ) {
    /* decode unsigned */
    uint64_t nv, ret = vbudecode( &nv, buffer );
    /* convert magnitude|sign to sign|magnitude */
    *value = ((nv >> 1) ^ -(nv & 1));
    return ret;
}

#ifdef TINYVBYTE_TEST
#include <stdio.h>
#define test(type, encfunc, decfunc, number) do { \
    type copy; \
    char buf[16]; \
    int written = encfunc( buf, number ), i = 0; \
    printf("[    ] %s: ", #type); \
    while( i < written ) printf("%02x,", (uint8_t)buf[i++] ); \
    decfunc( &copy, buf ); \
    printf("\r%s\n", copy == number ? "[ OK ]" : "[FAIL]"); \
} while(0)
void TINYVBYTE_TEST() {
    test( int64_t, vbiencode, vbidecode,  0);
    test( int64_t, vbiencode, vbidecode, -1);
    test( int64_t, vbiencode, vbidecode, +1);
    test( int64_t, vbiencode, vbidecode, -2);
    test( int64_t, vbiencode, vbidecode, +2);
    test( int64_t, vbiencode, vbidecode, INT8_MIN);
    test( int64_t, vbiencode, vbidecode, INT8_MAX);
    test( int64_t, vbiencode, vbidecode, INT16_MIN);
    test( int64_t, vbiencode, vbidecode, INT16_MAX);
    test( int64_t, vbiencode, vbidecode, INT32_MIN);
    test( int64_t, vbiencode, vbidecode, INT32_MAX);
    test( int64_t, vbiencode, vbidecode, INT64_MIN);
    test( int64_t, vbiencode, vbidecode, INT64_MAX);

    test(uint64_t, vbuencode, vbudecode, 0);
    test(uint64_t, vbuencode, vbudecode, 1);
    test(uint64_t, vbuencode, vbudecode, 2);
    test(uint64_t, vbuencode, vbudecode, 3);
    test(uint64_t, vbuencode, vbudecode, 4);
    test(uint64_t, vbuencode, vbudecode, UINT8_MAX);
    test(uint64_t, vbuencode, vbudecode, UINT16_MAX);
    test(uint64_t, vbuencode, vbudecode, UINT32_MAX);
    test(uint64_t, vbuencode, vbudecode, UINT64_MAX);
}
#endif
