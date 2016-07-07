// tiny de/humanized numbers. based on freebsd implementation.
// - rlyeh, public domain | wtrmrkrlyeh

#pragma once
#include <string>
#include <stdint.h>

inline std::string humanize( uint64_t num, const char *suffix = " " ) {
    const char prefixes[] = " KMGTPE";
    const char* prefixp = prefixes;
    uint64_t i = num, d = 0;
    while( (i > 1024) && *prefixp++ ) {
        d = (i % 1024) / 10, i /= 1024;
    }
    if (d > 0) return std::to_string(i) + '.' + std::to_string(d) + suffix + *prefixp;
    else       return std::to_string(i) + suffix + *prefixp;
}

inline uint64_t dehumanize( const std::string &str ) {
    size_t sz = 1, mul = 0;
    double num = std::stof(str, &sz);
    while( str[sz] && str[sz] == ' ' ) sz++;
    switch( str[sz] ) {
        default: case 'B': case 'b': mul = 0;
        break;   case 'K': case 'k': mul = 1;
        break;   case 'M': case 'm': mul = 2;
        break;   case 'G': case 'g': mul = 3;
        break;   case 'T': case 't': mul = 4;
        break;   case 'P': case 'p': mul = 5;
        break;   case 'E': case 'e': mul = 6; // may overflow
    }
    while( mul-- ) if( num * 1024 < num ) return 0; else num *= 1024;
    return num;
}

#ifdef TINYHUMAN_BUILD_DEMO
#include <iostream>
int main() {
    std::cout << __LINE__ << " " << humanize(1238) << "m" << std::endl;
    std::cout << __LINE__ << " " << humanize(123823) << "l" << std::endl;
    std::cout << __LINE__ << " " << humanize(123828328) << "bytes" << std::endl;
    std::cout << "---" << std::endl;

    std::cout << __LINE__ << " " << dehumanize("118 km") << std::endl;
    std::cout << __LINE__ << " " << dehumanize("118.9M") << std::endl;
    std::cout << __LINE__ << " " << dehumanize("118M") << std::endl;
    std::cout << __LINE__ << " " << dehumanize("118b") << std::endl;
    std::cout << __LINE__ << " " << dehumanize("118k") << std::endl;
    std::cout << __LINE__ << " " << dehumanize("118mb") << std::endl;
    std::cout << __LINE__ << " " << dehumanize("118gb") << std::endl;
    std::cout << __LINE__ << " " << dehumanize("118tb") << std::endl;
    std::cout << __LINE__ << " " << dehumanize("118pb") << std::endl;
}
#endif
