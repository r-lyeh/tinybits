// tiny variable-length encoder/decoder (vbyte)
// - rlyeh, public domain
#pragma once
#include <stdint.h>

enum { VBYTE_MIN_REQ_BYTES = 1, VBYTE_MAX_REQ_BYTES = 10 };

static uint64_t tinyvbyte_uencode( uint8_t *buffer, uint64_t value ) {
    /* 7-bit packing. MSB terminates stream */
    const uint8_t *buffer0 = buffer;
    do {
        *buffer++ = (uint8_t)( 0x80 | (value & 0x7f) );
        value >>= 7;
    } while( value > 0 );
    *(buffer-1) ^= 0x80;
    return buffer - buffer0;
}
static uint64_t tinyvbyte_idecode( uint64_t *value, const uint8_t *buffer ) {
    /* 7-bit unpacking. MSB terminates stream */
    const uint8_t *buffer0 = buffer;
    uint64_t out = 0, j = -7;
    do {
        out |= (( ((uint64_t)(*buffer)) & 0x7f) << (j += 7) );
    } while( ((uint64_t)(*buffer++)) & 0x80 );
    *value = out;
    return buffer - buffer0;
}

static uint64_t tinyvbyte_encode_i( uint8_t *buffer, int64_t value ) {
    /* convert sign|magnitude to magnitude|sign */
    uint64_t nv = (uint64_t)value;
    nv = nv & (1ull << 63) ? ~(nv << 1) : (nv << 1);
    /* encode unsigned */
    return tinyvbyte_uencode( buffer, nv );
}
static uint64_t tinyvbyte_decode_i( int64_t *value, const uint8_t *buffer ) {
    /* decode unsigned */
    uint64_t nv, ret = tinyvbyte_idecode( &nv, buffer );
    /* convert magnitude|sign to sign|magnitude */
    *value = nv & (1) ? ~(nv >> 1) : (nv >> 1);
    return ret;
}
