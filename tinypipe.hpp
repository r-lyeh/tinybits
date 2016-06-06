// tiny chainable pipes (C++11)
// - rlyeh, public domain

#pragma once
#include <vector>
#include <functional>

/* pipe manipulation */
template< typename out_t, typename in_t >
struct pipe {
    std::vector< std::function<bool(out_t&,const in_t&)> > filters;

    // basics
    template<typename filter>
    pipe &operator <<( const filter &fn ) {
        return filters.push_back( fn ), *this;
    }
    bool operator()( out_t &out, const in_t &in ) const {
        enum state { FAIL, KEEP, STOP } st = KEEP;
        for( auto &fn : filters ) {
            if( KEEP != ( st = (state)fn( out, in ) ) ) {
                break;
            }
        }
        return FAIL != st;
    }
    out_t operator()( const in_t &in ) const {
        out_t out;
        return operator()( out, in ) ? out : out_t();
    }

    // aliases
    template<typename filter>
    pipe &chain( const filter &fn ) {
        return operator <<( fn );
    }
    bool process( out_t &out, in_t &in ) const {
        return operator()( out, in );
    }
    out_t process( const in_t &in ) const {
        return operator()( in );
    }
};

#ifdef TINYPIPE_BUILD_DEMO
#include <cassert>
#include <string>
#include <iostream>
int main() {
    pipe< std::string, std::string > in, out;

    // setup
    in  << []( std::string &dst, const std::string &src ) { return dst = src + "1", true; }
        << []( std::string &dst, const std::string &src ) { return dst += "2", true; }
        << []( std::string &dst, const std::string &src ) { return std::reverse( dst.begin(), dst.end() ), true; };

    out << []( std::string &dst, const std::string &src ) { return dst = src, std::reverse( dst.begin(), dst.end() ), true; }
        << []( std::string &dst, const std::string &src ) { return dst.pop_back(), true; }
        << []( std::string &dst, const std::string &src ) { return dst.pop_back(), true; };

    std::string indata = "hello";
    std::cout << indata            << " --> " << in( indata ) << std::endl;
    std::cout << out(in( indata )) << " <-- " << in( indata ) << std::endl;

    assert( indata == out(in(indata)) );
}
#endif
