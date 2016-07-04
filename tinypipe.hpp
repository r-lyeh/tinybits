// tiny chainable pipes (C++11)
// - rlyeh, public domain

#pragma once
#include <vector>
#include <sstream>

template< typename T, typename istream >
void copy( T &s, const istream &is ) {
    std::stringstream ss;
    std::streamsize at = is.rdbuf()->pubseekoff(0,is.cur);
    ss << is.rdbuf();
    is.rdbuf()->pubseekpos(at);
    auto x = ss.str();
    s.assign( x.begin(), x.end() );
}

template< typename istream, typename ostream, typename container = std::vector<char> >
bool pipe( const istream &is, ostream &os, const std::vector< int (*)(const char *, int, char *, int) > &vec = std::vector< int (*)(const char *, int, char *, int) >() ) {
    container src, dst;
    container *A = &src, *B = &dst, *C;
    copy( src, is );
    if( !is.good() ) {
        return false;
    }
    for( auto &fn : vec ) {
        auto bounds = fn( (const char *)&(*A)[0],A->size(),0,B->size() );
        B->resize( bounds );
        int wlen = fn( (const char *)&(*A)[0], A->size(), &(*B)[0], B->size() );
        if( wlen >= 0 ) {
            B->resize( wlen );
            C = B, B = A, A = C;
        } else return false;
    }
    os.write( &(*A)[0], (*A).size() );
    return os.good();
}


#ifdef TINYPIPE_BUILD_DEMO
int rot13ish( const char *src, int slen, char *dst, int dlen ) {
    if( !dst ) return slen * 2; // bounds
    char *bak = dst;
    const char *p = src, *e = src + slen;
    while( p < e ) *dst++ = (*p & 0xf0) | (*p & 0x0f) ^ 0x7, ++p;
    return dst - bak;
}
int upper( const char *src, int slen, char *dst, int dlen ) {
    if( !dst ) return slen * 2; // bounds
    char *bak = dst;
    const char *p = src, *e = src + slen;
    size_t len = e - p;
    while( p < e ) *dst++ = (*p >= 'a' ? *p - 'a' + 'A' : *p), ++p;
    return dst - bak;
}
int lower( const char *src, int slen, char *dst, int dlen ) {
    if( !dst ) return slen * 2; // bounds
    char *bak = dst;
    const char *p = src, *e = src + slen;
    size_t len = e - p;
    while( p < e ) *dst++ = (*p >= 'A' ? *p - 'A' + 'a' : *p), ++p;
    return dst - bak;
}
int noparens( const char *src, int slen, char *dst, int dlen ) {
    if( !dst ) return slen * 2; // bounds
    char *bak = dst;
    const char *p = src, *e = src + slen;
    while( p < e ) {
        if( *p != '(' && *p != ')' ) *dst++ = *p++; else *p++;
    }
    return dst - bak;
}
int numberx2( const char *src, int slen, char *dst, int dlen ) {
    if( !dst ) return slen * 2; // bounds
    char *bak = dst;
    const char *p = src, *e = src + slen;
    while( p < e ) {
        bool n = *p >= '0' && *p <= '9';
        dst[0]=dst[n]=*p++, dst+=n+1;
    }
    return dst - bak;
}
int l33t( const char *src, int slen, char *dst, int dlen ) {
    if( !dst ) return slen * 2; // bounds
    char *bak = dst;
    const char *p = src, *e = src + slen;
    while(p < e)
    switch(*p++) {
        default: *dst++ = p[-1];
        break; case 'O': *dst++ = '0';
        break; case 'T': *dst++ = '7';
        break; case 'E': *dst++ = '3';
        break; case 'L': *dst++ = '1';
        break; case 'I': *dst++ = '1';
        break; case 'A': *dst++ = '4';
    }
    return dst - bak;
}
#include <fstream>
#include <iostream>
#include <sstream>
int main() {
    std::stringstream is; is << "hello";
    pipe( is, std::cout );

    std::ifstream ifs{__FILE__};
    pipe( ifs, std::cout, { rot13ish, rot13ish } );
    pipe( ifs, std::cout, { upper, l33t, lower, numberx2, noparens } );
}
#endif
