// tiny iso/9660 unarchiver. [ref] http://wiki.osdev.org/ISO_9660
// - rlyeh, public domain | wtrmrkrlyeh
#pragma once
#include <functional>
#include <stdint.h>
#include <string>
#include <vector>

template<typename FN, typename istream>
bool tinyuniso( istream &is, const FN &yield ) {
    // directory record
    struct dir_t {
        uint8_t  flags;
        uint64_t offset;
        uint64_t length;
        uint16_t parent;
        union { uint8_t st[7]; struct { uint8_t Y,M,D,h,m,s,GMT; }; };
        std::string name;
    } root = {}; 

    // unpack data (raw)
    auto unpack_raw = [&]( int l, void *ptr ) {
        if( !ptr ) is.ignore( l ); else is.read( (char *)ptr, l );
    };

    // unpack directory record
    auto unpack_record = [&]( dir_t *out ) -> uint8_t {
        auto get_max_of = []( int a, int b ) { return a < b ? b : a; };
        auto unpack_16L = [&]( uint32_t *t ) { unpack_raw(4, t); if(t) *t &= (uint16_t)(-1); };
        auto unpack_32L = [&]( uint64_t *t ) { unpack_raw(8, t); if(t) *t &= (uint32_t)(-1); };
        auto unpack_str = [&]( int l, std::string *s ) {
            s->resize( l );
            unpack_raw( l, &(*s)[0] );
            while( !s->empty() && s->back() == ' ' ) s->pop_back();     // rstrip(' ')
            *s = s->substr( 0, s->find_first_of(';') );                 // left_of(';')
            for( auto &ch : *s ) ch -= 32 * ( ch >= 'a' && ch <= 'z' ); // upper()
        };
        uint8_t len0, len1;
        unpack_raw( 1, &len0 );
        if( len0 ) {
            unpack_raw( 1, 0 );
            unpack_32L( &(out->offset) );
            unpack_32L( &(out->length) );
            unpack_raw( 7, &(out->st) );
            unpack_raw( 1, &(out->flags) );
            unpack_raw( 1+1+4, 0 );
            unpack_raw( 1, &len1 );
            unpack_str( len1, &(out->name) );
            unpack_raw( !(len1 % 2), 0);
            unpack_raw( get_max_of(0, len0 - (34 + len1 - (len1 % 2))), 0 );
        } else len0 = 1, *out = dir_t();
        return len0;
    };

    // retrieve partial contents
    auto get_sector = [&]( int64_t sector, int64_t length ) {
        return is.seekg( sector*2048, is.beg ), is.good();
    };

    // retrieve whole file contents
    auto get_file = [&]( char *ptr, const dir_t &f ) {
        get_sector( f.offset, f.length );
        unpack_raw( f.length, ptr );
    };

    // parse volume descriptors
    int sector = 0x10;
    while( get_sector(sector++, 2048) ) {
        uint8_t typecode;
        unpack_raw(1, &typecode);

        /**/ if( typecode == 255 ) break;
        else if( typecode == 0x1 ) { // skip most of primary volume descriptor
            std::string id(5, '\0');
            unpack_raw(5, &id[0]); if(id != "CD001") return false;
            unpack_raw(1+1+32+32+8+8+32+4+4+4+8+4+4+4+4, 0);
            unpack_record( &root ); break;
        }
        else return false;
    }

    // recurse directory structure
    using fn = std::function<void (const dir_t &, const char *)>;
    fn tree = [&] ( const dir_t &node, const char *parent ) {
        std::vector<dir_t> list;
        // unpack dir
        auto sector = node.offset;
        auto read = 0;
        get_sector(sector, 2048);
        dir_t self; read += unpack_record( &self );
        dir_t prev; read += unpack_record( &prev );
        // iterate children
        while( read < self.length ) {
            if( read % 2048 == 0) {
                get_sector(++sector, 2048);
            }

            dir_t data;
            read += unpack_record( &data );

            if( data.name.empty() ) { // if end of directory
                auto skip = 2048 - (read % 2048);
                unpack_raw(skip, 0);
                read += skip;
            } else {
                list.push_back( data );
            }
        }
        // iterate listing: recurse if dir, else yield file
        for( auto &c : list ) {
            if( c.flags & 2 ) {
                tree( c, (node.name + "/").c_str() );
            } else {
                std::string fname = std::string("/") + parent + node.name + "/" + c.name;
                char buffer[128] = {};
                auto offset = (int)(c.GMT) * 15 * 60; // offset in seconds from GMT in 15min intervals
                sprintf(buffer, "%d/%02d/%02d %02d:%02d:%02d%c%d", 1900+c.Y, c.M, c.D, c.h, c.m, c.s, offset >= 0 ? '+':'-', offset );
                if( char *ptr = yield( fname, c.length, buffer, c.offset ) ) {
                    get_file( ptr, c );
                }
            }
        }
    };

    tree(root, "");
    return is.good();
}

/*
#include <iostream>
#include <fstream>
int main( int argc, const char **argv ) {
    if( argc <= 1 ) {
        return std::cout << "Usage: " << argv[0] << " file.iso [path]" << std::endl, -1;
    }
    std::string read;
    std::ifstream ifs( argv[1], std::ios::binary );
    bool ok = ifs.good() && tinyuniso( ifs, 
        [&]( const std::string &path, uint64_t size, const char *stamp, uint64_t offset ) { 
            // .csv list
            std::cout << "'" << path << "'," << size << ",'" << stamp  << "'" << std::endl;
            // read file (if provided)
            if( argc > 2 && path == argv[2] ) {
                read.resize( size );
                return &read[0];
            }
            return (char *)0;
        }
    );
    if( !read.empty() ) std::cout << read << std::endl;
    return ok;
}
*/
