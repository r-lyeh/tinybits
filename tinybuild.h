// Tiny buildinfo macros
// - rlyeh, public domain

#pragma once

#ifndef BUILD_GIT_BRANCH
#define BUILD_GIT_BRANCH   "n/a"
#endif

#ifndef BUILD_GIT_REVISION
#define BUILD_GIT_REVISION "NaN"
#endif

#ifndef BUILD_BITS
#if _WIN64 || __x86_64__ || __ppc64__
#define BUILD_BITS 64
#elif _WIN32 || __GNUC__
#define BUILD_BITS 32
#else
#define BUILD_BITS 00
#endif
#endif

#ifndef BUILD_PROJECT
#define BUILD_PROJECT "UNNAMED"
#endif

#ifndef BUILD_VERSION
#define BUILD_VERSION "0.0.0"
#endif

#ifndef BUILD_URL
#define BUILD_URL     "https://"
#endif

#ifndef BUILD_STAMP
#define BUILD_STAMP   __DATE__ " " __TIME__
#endif

#ifndef BUILD_STR
#define BUILD_STr(a)  #a
#define BUILD_STR(a)  BUILD_STr(a)
#endif

#ifndef BUILD_ARCH
#define BUILD_ARCH    BUILD_STR(BUILD_BITS) "-bit"
#endif

#ifndef BUILD_TYPE
#define BUILD_TYPE    "DEBUG"
#endif

#ifndef BUILD_INFO
#define BUILD_INFO    BUILD_PROJECT " " BUILD_VERSION " (" BUILD_ARCH " " BUILD_TYPE ") (" BUILD_STAMP ") (git:" BUILD_GIT_BRANCH " rev:" BUILD_GIT_REVISION ")"
#endif

/*
#include <stdio.h>
int main() {
    puts( BUILD_INFO );
}
*/
