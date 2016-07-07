// tiny zip unarchiver. based on junzip by Joonas Pihlajamaa
// - rlyeh, public domain | wtrmrkrlyeh
#pragma once
#include <stdint.h>
#include <vector>

#define STBI_NO_WRITE
#define STBI_NO_HDR
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

template<typename FN, typename istream>
bool tinyunzip( istream &is, const FN &yield ) {
    bool swaple = 0;
    auto swap16 = [&]( uint16_t t ) { return !swaple ? t : (t >>  8) | (t <<  8); };
    auto swap32 = [&]( uint32_t t ) { return !swaple ? t : (t >> 24) | (t << 24) | ((t >> 8) & 0xff00) | ((t & 0xff00) << 8); };

#pragma pack( push, 1 )
    struct global_file_header {
        uint32_t magic; // <--
        uint16_t version;
        uint16_t versionreq;
        uint16_t flags;
        uint16_t zmethod; // <--
        uint16_t modftme;
        uint16_t modfdte;
        uint32_t crc32;
        uint32_t size;
        uint32_t unsize;
        uint16_t namelen; // <-- filename length
        uint16_t xtralen; // <-- extra field length
        uint16_t commlen; // <-- comment filed length
        uint16_t beginvol;
        uint16_t attrint;
        uint32_t attrext;
        uint32_t offsrel; // <--
    } gfh = {};

    struct file_header {
        uint16_t zmethod; // <--
        uint16_t modftme;
        uint16_t modfdte;
        uint32_t crc32;
        uint32_t size;   // <--
        uint32_t unsize; // <--
        union  { uint32_t offset;  // <--
        struct { uint16_t namelen; // <--
                 uint16_t xtralen; // <--
        }; };
    } fh = {};

    struct local_file_header {
        uint32_t magic; // <--
        uint16_t versionreq;
        uint16_t flags;
        file_header fh;
    } lfh;

    struct end_record {
        uint32_t magic; // <-- 0x06054b50
        uint16_t volno; // <--
        uint16_t volnodir; // <--
        uint16_t volentries; // <--
        uint16_t totentries; // <--
        uint32_t dirsize;
        uint32_t diroffs; // <--
        uint16_t commlen; // after this field, zip file comment follows (variable size)
    } er = {};
#pragma pack( pop )

    enum { BUFFER_SIZE = 65536 }; // max name len
    std::vector<char> buffer( BUFFER_SIZE ), cdata;

    auto read_end_record = [&] {
        is.seekg( 0, is.end );
        size_t total = is.tellg();
        if( total >= sizeof(end_record) ) {
            size_t to_read = (total < BUFFER_SIZE) ? total : BUFFER_SIZE;
            is.seekg( total - to_read, is.beg );
            is.read( &buffer[0], to_read );
            for( int at = to_read - sizeof(end_record); at >= 0; at-- ) {
                er = *(end_record *) &buffer[at];
                if( er.magic == 0x06054B50 || er.magic == 0x504B5460 ) {
                    swaple = er.magic != 0x06054B50;
                    return !swap16(er.volno) && !swap16(er.volnodir) && swap16(er.totentries) == swap16(er.volentries);
                }
            }
        }
        return false;
    };

    auto read_central_dir = [&] {
        is.seekg( swap32(er.diroffs), is.beg );
        if( is.good() ) for( int it = 0; it < swap16(er.totentries); it++ ) {
            is.read( (char *)&gfh, sizeof(global_file_header) );
            if( is.good() && gfh.magic == swap32(0x02014B50) && swap16(gfh.namelen) + 1 < BUFFER_SIZE ) {
                is.read( (char *)&buffer[0], swap16(gfh.namelen) );
                buffer[ swap16(gfh.namelen) ] = '\0';
                is.seekg( swap16(gfh.xtralen) + swap16(gfh.commlen), is.cur );
                fh = *(file_header *)&gfh.zmethod;
                fh.offset = gfh.offsrel;
                if( swap32(fh.size) != 0 || buffer[ swap16(gfh.namelen) - 1 ] != '/' )   // if not dir
                if( swap16(fh.zmethod) == 0 || swap16(fh.zmethod) == 8 ) {               // if supported
                    char *ptr = yield( &buffer[0], swap32(fh.unsize), swap32(fh.size) ); // reserve
                    if( ptr ) {
                        auto pos = is.tellg();
                        is.seekg( swap32(fh.offset), is.beg );
                        is.read( (char *)&lfh, sizeof(local_file_header) );
                        if( lfh.magic != swap32(0x04034B50) ) return false;
                        is.seekg(swap16(lfh.fh.namelen) + swap16(lfh.fh.xtralen), is.cur);
                        if( swap16(fh.zmethod) ) {
                            cdata.resize( swap32(fh.size) );
                            is.read( &cdata[0], cdata.size() );
                            if( stbi_zlib_decode_noheader_buffer( ptr, swap32(fh.unsize), &cdata[0], swap32(fh.size) ) < 0 ) {
                                return false;
                            }
                        } else {
                            is.read( ptr, swap32(fh.size) );
                        }
                        is.seekg( pos, is.beg );
                    }
                }
            } else return false;
        } else return false;
        return true;
    };

    return read_end_record() && read_central_dir() && is.good();
}

#ifdef TINYUNZIP_MAIN
#include <fstream>
#include <iostream>
#include <map>
int TINYUNZIP_MAIN( int argc, const char **argv ) {
    std::map<std::string, char *> toc;
    std::ifstream is( argc > 1 ? argv[1] : argv[0], std::ios::binary );
    return tinyunzip( is, [&]( const char *filename, int dlen, int zlen ) {
        std::cout << filename << " " << zlen << " -> " << dlen << std::endl;
        return toc[ filename ] = (char *)malloc(dlen);
    } );
}
#endif
