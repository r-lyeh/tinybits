// tiny directory listing
// - rlyeh, public domain | wtrmrkrlyeh
#pragma once
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <dirent.h>
#endif

template<typename FN>
bool tinydir( const char *directory, const FN &yield ) {
    std::string src( directory );
    while( !src.empty() && (src.back() == '/' || src.back() == '\\') ) src.pop_back();
#ifdef _WIN32
    WIN32_FIND_DATA fdata;
    for( HANDLE h = FindFirstFileA( (src + "/*").c_str(), &fdata ); h != INVALID_HANDLE_VALUE; ) {
        for( bool next = true; next; next = FindNextFileA( h, &fdata ) != 0 ) {
            if( fdata.cFileName[0] != '.' ) {
                yield( (src + "/" + fdata.cFileName).c_str(), (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0 );
            }
        }
        return FindClose( h ), true;
    }
#else
    for( DIR *dir = opendir( (src + "/").c_str() ); dir; ) {
        for( struct dirent *ep; ep = readdir( dir ); ) {
            if( ep->d_name[0] != '.' ) {
                DIR *tmp = opendir( ep->d_name );
                yield( (src + "/" + ep->d_name).c_str(), tmp ? (closedir( tmp ), 1) : 0 );
            }
        }
        return closedir( dir ), true;
    }
#endif
    return false;
}

#ifdef TINYDIR_MAIN
#include <stdio.h>
#include <functional>
void TINYDIR_MAIN() {
    std::function<void(const char *,bool)> callback = [&]( const char *name, bool is_dir ) {
        printf( "%5s %s\n", is_dir ? "<dir>" : "", name );
        //if( is_dir ) tinydir( name, callback ); // <-- uncomment for recursive listing
    };
    return tinydir( "./", callback );
}
#endif
