// Tiny hexdump viewer. rlyeh, public domain | wtrmrkrlyeh
#include <stdio.h>

void hexdump( FILE *fp, const void *ptr, unsigned len, int width ) {
    unsigned char *data = (unsigned char*)ptr;
    for( unsigned jt = 0; jt < len; jt += width ) {
        fprintf( fp, "; %05d ", jt );
        for( unsigned it = jt, next = it + width; it < len && it < next; ++it ) {
            fprintf( fp, "%02x %s", (unsigned char)data[it], &" \n\0...\n"[ (1+it) < len ? 2 * !!((1+it) % width) : 3 ] );
        }
        fprintf( fp, "; %05d ", jt );
        for( unsigned it = jt, next = it + width; it < len && it < next; ++it ) {
            fprintf( fp, " %c %s", (signed char)data[it] >= 32 ? (signed char)data[it] : (signed char)'.', &" \n\0...\n"[ (1+it) < len ? 2 * !!((1+it) % width) : 3 ] );
        }
    }
}


#include <string.h>
int main() {
    const char *sample = __FILE__ "/" __TIME__ "/" __DATE__;
    hexdump( stdout, sample, strlen(sample), 16 );
}
