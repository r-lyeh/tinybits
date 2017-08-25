// ini+, extended ini format 
// - rlyeh, public domain
//
// # spec
//
//   ; line comment
//   [details]          ; map section name (optional)
//   user=john          ; key and value (mapped here as details.user=john)
//   +surname=doe jr.   ; sub-key and value (mapped here as details.user.surname=doe jr.)
//   color=240          ; key and value \
//   color=253          ; key and value |> array: color[0], color[1] and color[2]
//   color=255          ; key and value /
//   color=             ; remove key/value(s)
//   color=white        ; recreate key; color[1] and color[2] no longer exist
//   []                 ; unmap section
//   -note=keys may start with symbols (except plus and semicolon)
//   -note=linefeeds are either \r, \n or \r\n.
//   -note=utf8 everywhere.
//

#pragma once

// api

char *ini( const char *text );

// api, alt callback version

void ini_cb( const char *text, void (*yield)( const char *key, const char *value, void *userdata ), void *userdata );

// impl

#include <stdio.h>
#include <stdlib.h>

static char *ini( const char *s ) {
    char *map = 0;
    int mapcap = 0, maplen = 0;
    enum { DEL, REM, TAG, KEY, SUB, VAL } fsm = DEL;
    const char *cut[6] = {0}, *end[6] = {0};
    while( *s ) {
        while( *s && (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') ) ++s;
        /**/ if( *s == ';' ) cut[fsm = REM] = ++s;
        else if( *s == '[' ) cut[fsm = TAG] = ++s;
        else if( *s == '+' ) cut[fsm = SUB] = ++s;
        else if( *s == '=' ) cut[fsm = VAL] = ++s;
        else if( *s > ' ' && *s <= 'z' && *s != ']' ) cut[fsm = KEY] = cut[SUB] = end[SUB] = s;
        else { ++s; continue; }
        /**/ if( fsm == REM ) { while(*s && *s != '\r'&& *s != '\n') ++s; }
        else if( fsm == TAG ) { while(*s && *s != '\r'&& *s != '\n'&& *s != ']') ++s; end[fsm] = s; }
        else if( fsm == KEY ) { while(*s && *s >  ' ' && *s <= 'z' && *s != '=') ++s; end[fsm] = s; }
        else if( fsm == SUB ) { while(*s && *s >  ' ' && *s <= 'z' && *s != '=') ++s; end[fsm] = s; }
        else if( fsm == VAL ) { while(*s && *s >= ' ' && *s <= 'z' && *s != ';') ++s; end[fsm] = s;
            while( end[fsm][-1] == ' ' ) --end[fsm];
            char buf[256] = {0}, *key = buf;
            if( end[TAG] - cut[TAG] ) key += sprintf(key, "%.*s.", end[TAG] - cut[TAG], cut[TAG] );
            if( end[KEY] - cut[KEY] ) key += sprintf(key,  "%.*s", end[KEY] - cut[KEY], cut[KEY] );
            if( end[SUB] - cut[SUB] ) key += sprintf(key, ".%.*s", end[SUB] - cut[SUB], cut[SUB] );
            int reqlen = (key - buf) + 1 + (end[VAL] - cut[VAL]) + 1 + 1;
            if( (reqlen + maplen) >= mapcap ) map = realloc( map, mapcap += reqlen + 512 );
            sprintf( map + maplen, "%.*s%c%.*s%c%c", key - buf, buf, 0, end[VAL] - cut[VAL], cut[VAL], 0, 0 );
            maplen += reqlen - 1;
        }
    }
    return map;
}

static void ini_cb( const char *text, void (*yield)( const char *key, const char *value, void *userdata ), void *userdata ) {
    char *kv = ini( text );
    if( kv ) {
        for( char *iter = kv; iter[0]; ) {
            const char *key = iter; while( *iter++ );
            const char *val = iter; while( *iter++ );
            yield( key, val, userdata );
        }
        free( kv );
    }
}


#ifdef TINYINI_DEMO

void TINYINI_DEMO() {

    char *kv = ini(
        "; line comment\n"
        "[details]          ; map section name (optional)\n"
        "user=john          ; key and value (mapped here as details.user=john)\n"
        "+surname=doe jr.   ; sub-key and value (mapped here as details.user.surname=doe jr.)\n"
        "color=240          ; key and value \\\n"
        "color=253          ; key and value |> array: color[0], color[1] and color[2]\n"
        "color=255          ; key and value /\n"
        "color=             ; remove key/value(s)\n"
        "color=white        ; recreate key; color[1] and color[2] no longer exist\n"
        "[]                 ; unmap section\n"
        "-note=keys may start with symbols (except plus and semicolon)\n"
        "-note=linefeeds are either \\r, \\n or \\r\\n.\n"
        "-note=utf8 everywhere.\n"
    );

    if( kv ) {
        for( char *iter = kv; iter[0]; ) {
            printf("key: '%s'\n", iter); while( *iter++ );
            printf("val: '%s'\n", iter); while( *iter++ );
        }
        free( kv );
    }
}

#endif
