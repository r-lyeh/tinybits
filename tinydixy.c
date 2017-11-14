// tinydixy, small hierarchical config file format (subset of YAML, spec in https://github.com/kuyawa/Dixy)
// - rlyeh, public domain

// api, returns number of pairs found

int tinydixy( const char *buffer, int (*yield)(const char *key, const char *val) );

// implementation

#include <stdio.h>
#include <stdlib.h>

int tinydixy( const char *s, int (*yield)(const char *key, const char *val) ) {
    char *map = 0;
    int mapcap = 0, maplen = 0, num_pairs_found = 0;
    enum { DEL, REM, KEY, SUB, VAL } fsm = DEL;
    const char *cut[5] = {0}, *end[5] = {0};
    while( *s ) {
        while( *s && (*s == '\r' || *s == '\n') ) ++s;
        /**/ if( *s == '#' ) cut[fsm = REM] = ++s;
        else if( *s == ' ' || *s == '\t' ) cut[fsm = SUB] = ++s;
        else if( *s == ':' ) cut[fsm = VAL] = ++s;
        else if( *s > ' ' && *s <= 'z' ) cut[fsm = KEY] = cut[SUB] = end[SUB] = s, free(map), map = 0, mapcap = 0, maplen = 0;
        else { ++s; continue; }
        /**/ if( fsm == REM ) { while(*s && *s != '\r'&& *s != '\n') ++s; }
        else if( fsm == KEY ) { while(*s && *s >  ' ' && *s <= 'z' && *s != ':') ++s; end[fsm] = s; }
        else if( fsm == SUB ) { while(*s && *s >  ' ' && *s <= 'z' && *s != ':') ++s; end[fsm] = s; }
        else if( fsm == VAL ) { while(*s && *s >= ' ' && *s <= 'z' && *s != '\r' && *s != '\n') ++s; end[fsm] = s;
            while( end[fsm][-1] == ' ' ) --end[fsm];
            char buf[256] = {0}, *key = buf, *val = "";
            if( end[KEY] - cut[KEY] ) key += sprintf(key,  "%.*s", end[KEY] - cut[KEY], cut[KEY] );
            if( end[SUB] - cut[SUB] ) key += sprintf(key, ".%.*s", end[SUB] - cut[SUB], cut[SUB] );
            int reqlen = (key - buf) + 1 + (end[VAL] - cut[VAL]) + 1 + 1;
            if( (reqlen + maplen) >= mapcap ) map = realloc( map, mapcap += reqlen + 512 );
            sprintf( map + maplen, "%.*s%c%.*s%c%c", key - buf, buf, 0, end[VAL] - cut[VAL], cut[VAL], 0, 0 );
            val = map + maplen + (key - buf) + 2, key = map + maplen;
            if( val[0] ) { yield( key, val ); num_pairs_found++; }
            maplen += reqlen - 1;
        }
    }
    free( map );
    return num_pairs_found;
}

#ifdef TINYDIXY_DEMO

// sample

int puts2( const char *key, const char *val ) {
    printf("%s:'%s'\n", key, val);
    return 0;
}

int main() {
    const char *sample = 
    "# Dixy 1.0\n"
    "\n"
    "name: Taylor Swift\n"
    "age: 27\n"
    "phones:\n"
    "    0: 555-SWIFT\n"
    "    1: 900-SWIFT\n"
    "    2: 800-TAYLOR\n"
    "body:\n"
    "    height: 6 ft\n"
    "    weight: 120 lbs\n"
    "pets:\n"
    "    0:\n"
    "        name: Fido\n"
    "        breed: chihuahua\n"
    "    1:\n"
    "        name: Tinkerbell\n"
    "        breed: bulldog\n";

    printf("%d keys found\n", tinydixy( sample, puts2 ));
}

#endif
