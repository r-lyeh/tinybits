// portable gnu tar and ustar extraction
// - rlyeh, public domain | wtrmrkrlyeh
#pragma once
#include <string>
#include <stdint.h>

template<typename FN, typename istream>
bool tinyuntar( istream &is, const FN &yield ) {
    enum {
        name     =   0, // (null terminated)
        mode     = 100, // (octal)
        uid      = 108, // (octal)
        gid      = 116, // (octal)
        size     = 124, // (octal)
        modtime  = 136, // (octal)
        checksum = 148, // (octal)
        type     = 156, // \0|'0':file,1:hardlink,2:symlink,3:chardev,4:blockdev,5:dir,6:fifo,L:longnameblocknext
        linkname = 157, // if !ustar link indicator
        magic    = 257, // if ustar "ustar" -- 6th character may be space or null, else zero
        version  = 263, // if ustar "00", else zero
        uname    = 265, // if ustar owner username, else zero
        gname    = 297, // if ustar owner groupname, else zero
        devmajor = 329, // if ustar device major number, else zero
        devminor = 337, // if ustar device minor number , else zero
        path     = 345, // if ustar filename prefix, else zero
        padding  = 500, // if ustar relevant for checksum, else zero
        total    = 512
    };

    // equivalent to sscanf(buf, 8, "%.7o", &size); or (12, "%.11o", &modtime)
    // ignores everything after first null or space, including trailing bytes
    struct octal { 
        uint64_t operator()( const char *src, const char *eof ) const {
            uint64_t sum = 0, mul = 1;
            const char *ptr = eof;
            while( ptr-- >= src ) eof  = ( 0 != ptr[1] && 32 != ptr[1] ) ? eof : ptr;
            while( eof-- >= src ) sum += (uint8_t)(eof[1] - '0') * mul, mul *= 8;
            return sum;
    } };

    // handle both regular tar and ustar tar filenames until end of tar is found
    for( char header[512], blank[512] = {}; is.good(); ) {
        is.read( header, 512 );
        if( memcmp( header, blank, 512 ) ) {                                      // if not end of tar
            if( !memcmp( header+magic, "ustar", 5 ) ) {                           // if valid ustar
                int namelen = strlen(header+name), pathlen = strlen(header+path); // read filename
                std::string entry =
                std::string(header+path, pathlen < 155 ? pathlen : 155 ) + (pathlen ? "/" : "") +
                std::string(header+name, namelen < 100 ? namelen : 100 );

                switch( header[type] ) {
                    default:                                                      // unsupported file type
                    break; case '5': //yield(entry.back()!='/'?entry+'/':entry,0);// directory
                    break; case 'L': entry = header+name; is.read( header, 512 ); // gnu tar long filename
                    break; case '0': case 0: {                                    // regular file
                        uint64_t len = octal()(header+size, header+modtime);      // decode octal size
                        char *dst = (len ? yield(entry, len) : 0);                // read block if processed
                        if( dst ) is.read( dst, len ); else is.ignore( len );     // skip block if unprocessed
                        is.ignore( (512 - (len & 511)) & 511 );                   // skip padding
                    }
                }
            } else return false;
        } else return is.good();
    }

    return false;
}

/*
#include <iostream>
#include <fstream>
#include <map>
int main( int argc, const char **argv ) {
    if( argc != 2 ) return std::cerr << "Usage: " << argv[0] << " archive.tar" << std::endl, -1;
    std::map<std::string, char *> dir;
    std::ifstream in(argv[1], std::ios_base::binary);
    return tinyuntar( in, [&]( const std::string &filename, uint64_t size ) {
        std::cout << filename << " (" << size << " bytes)" << std::endl;
        return dir[ filename ] = (char *)malloc( size ); // processed if valid ptr, skipped if null
    } );
}
*/
