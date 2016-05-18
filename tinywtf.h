// tiny portable host macros (C/C++ language features, compilers, os, arch, tls...)
// used to avoid #ifn/def hell.
// - rlyeh, public domain.

#ifndef $no

// Core

#define $no(...)
#define $yes(...)        __VA_ARGS__

// Directive modifiers

#define $on(v)          (0 v(+1))  // usage: #if $on($cl)
#define $is             (0 v(+1))  // usage: #if $is($debug)
#define $has(...)       $clang(__has_feature(__VA_ARGS__)) $nclang(__VA_ARGS__) // usage: #if $has(cxx_exceptions)

// Text/code modifiers

#define $quote(...)     #__VA_ARGS__
#define $comment        $no
#define $uncomment      $yes

// Compiler utils

#include <stdint.h>
#if   INTPTR_MAX == INT64_MAX
#   define $bits64    $yes
#   define $bits32    $no
#elif INTPTR_MAX == INT32_MAX
#   define $bits64    $no
#   define $bits32    $yes
#else
#   define $bits64    $no
#   define $bits32    $no
#endif

#if defined(NDEBUG) || defined(_NDEBUG) || defined(RELEASE)
#   define $release   $yes
#   define $debug     $no
#else
#   define $release   $no
#   define $debug     $yes
#endif

#if defined(NDEVEL) || defined(_NDEVEL) || defined(PUBLIC)
#   define $public    $yes
#   define $devel     $no
#else
#   define $public    $no
#   define $devel     $yes
#endif

#if defined(__GNUC__) || defined(__MINGW32__)
#   define $gcc       $yes
#   define $ngcc      $no
#else
#   define $gcc       $no
#   define $ngcc      $yes
#endif

#ifdef _MSC_VER
#   define $cl        $yes
#   define $ncl       $no
#else
#   define $cl        $no
#   define $ncl       $yes
#endif

#ifdef __clang__
#   define $clang     $yes
#   define $nclang    $no
#else
#   define $clang     $no
#   define $nclang    $yes
#endif

#if $on($cl) || $on($gcc) || $on($clang)
#   define $supported_compiler   $yes
#   define $unsupported_compiler $no
#else
#   define $supported_compiler   $no
#   define $unsupported_compiler $yes
#endif

// usage: if $likely (expr) { ... }

#if $on($gcc) || $on($clang)
#   define $likely(expr)    (__builtin_expect(!!(expr), 1))
#   define $unlikely(expr)  (__builtin_expect(!!(expr), 0))
#else
#   define $likely(expr)    ((expr))
#   define $unlikely(expr)  ((expr))
#endif

// create a $warning(...) macro
// usage: $warning("this is shown at compile time")

#if   $on($gcc) || $on($clang)
#   define $w4rning(msg) _Pragma(#msg)
#   define $warning(msg) $w4rning( message( msg ) )
#elif $on($cl)
#   define $warning(msg) __pragma( message( msg ) )
#else
#   define $warning(msg)
#endif

// create a $todo(...) macro
// usage: $todo("this is shown at compile time")

#define $t0d0(X)   #X
#define $tod0(X)   $t0d0(X)
#define $todo(...) $warning( __FILE__ "(" $tod0(__LINE__)") : $todo - " #__VA_ARGS__ " - [ " $cl(__FUNCTION__) $ncl(__func__) " ]" )

// C++ detect and version

#ifdef __cplusplus
#   define $c              $no
#   define $cpp            $yes
#   if (__cplusplus < 201103L && !defined(_MSC_VER)) || (defined(_MSC_VER) && (_MSC_VER < 1700)) || (defined(__GLIBCXX__) && __GLIBCXX__ < 20130322L)
#      define $cpp11       $no
#      define $cpp03       $yes
#   else
#      define $cpp11       $yes
#      define $cpp03       $no
#   endif
#else
#   define $c              $yes
#   define $cpp            $no
#   define $cpp11          $no
#   define $cpp03          $no
#endif

// C++ exceptions

#if defined(__cplusplus) && ( \
    (defined(_HAS_EXCEPTIONS) && (_HAS_EXCEPTIONS > 0)) || \
    (defined(_STLP_USE_EXCEPTIONS) && (_STLP_USE_EXCEPTIONS > 0)) || \
    (defined(HAVE_EXCEPTIONS)) || \
    (defined(__EXCEPTIONS)) || \
    (defined(_CPPUNWIND)) || \
    ($has(cxx_exceptions)) ) /*(__has_feature(cxx_exceptions))*/
#   define $exceptions    $yes
#   define $nexceptions   $no
#else
#   define $exceptions    $no
#   define $nexceptions   $yes
#endif

// Thread Local Storage

#if defined(__MINGW32__) || defined(__SUNPRO_C) || defined(__xlc__) || defined(__GNUC__) || defined(__clang__) || defined(__GNUC__) // __INTEL_COMPILER on linux
//   MingW, Solaris Studio C/C++, IBM XL C/C++,[3] GNU C,[4] Clang[5] and Intel C++ Compiler (Linux systems)
#    define $tls(x) __thread x
#else
//   Visual C++,[7] Intel C/C++ (Windows systems),[8] C++Builder, and Digital Mars C++
#    define $tls(x) __declspec(thread) x
#endif

// OS utils

#if defined(_WIN32)
#   define $windows      $yes
#   define $nwindows     $no
#else
#   define $windows      $no
#   define $nwindows     $yes
#endif

#ifdef __linux__
#   define $linux        $yes
#   define $nlinux       $no
#else
#   define $linux        $no
#   define $nlinux       $yes
#endif

#ifdef __APPLE__
#   define $apple        $yes
#   define $napple       $no
#else
#   define $apple        $no
#   define $napple       $yes
#endif

#if defined(__APPLE__) && defined(TARGET_OS_MAC)
#   define $osx          $yes
#   define $nosx         $no
#else
#   define $osx          $no
#   define $nosx         $yes
#endif

#if defined(__APPLE__) && (defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))
#   define $ios          $yes
#   define $nios         $no
#else
#   define $ios          $no
#   define $nios         $yes
#endif

#if defined(__ANDROID_API__)
#   define $android      $yes
#   define $nandroid     $no
#else
#   define $android      $no
#   define $nandroid     $yes
#endif

#if $on($windows) || $on($apple) || $on($linux) || $on($osx) || $on($ios) || $on($android)
#   define $supported_os         $yes
#   define $unsupported_os       $no
#else
#   define $supported_os         $no
#   define $unsupported_os       $yes
#endif

#endif

#ifdef UNDEFINE
#undef UNDEFINE
#undef $android
#undef $apple
#undef $bits32
#undef $bits64
#undef $c
#undef $cl
#undef $clang
#undef $comment
#undef $cpp
#undef $cpp03
#undef $cpp11
#undef $debug
#undef $devel
#undef $exceptions
#undef $gcc
#undef $has
#undef $ios
#undef $is
#undef $likely
#undef $linux
#undef $nandroid
#undef $napple
#undef $ncl
#undef $nclang
#undef $nexceptions
#undef $ngcc
#undef $nios
#undef $nlinux
#undef $no
#undef $nosx
#undef $nwindows
#undef $on
#undef $osx
#undef $public
#undef $quote
#undef $release
#undef $supported_compiler
#undef $supported_os
#undef $tls
#undef $todo
#undef $uncomment
#undef $unlikely
#undef $unsupported_compiler
#undef $unsupported_os
#undef $warning
#undef $windows
#undef $yes
#ifdef $tod0
#undef $tod0
#endif
#ifdef $t0d0
#undef $t0d0
#endif
#ifdef $w4rning
#undef $w4rning
#endif
#endif

/*
#include <stdio.h>
int main() {
    $c(
        puts("C");
    )
    $cpp(
        puts("C++");
        $cpp03(
            puts("C++03");
        )
        $cpp11(
            puts("C++11");
        )
        $exceptions(
            puts("exceptions enabled");
        )
        $nexceptions(
            puts("exceptions disabled");
        )
    )

    $bits32(
        puts("32-bit");
    )
    $bits64(
        puts("64-bit");
    )

    $debug(
        puts("unoptimized");
    )
    $release(
        puts("optimized");
    )

    $windows(
        puts("windows");
    )
    $ios(
        puts("ios");
    )
    $osx(
        puts("osx");
    )

    $warning("this line emits a warning");
    $todo("this line needs a better comment");

    const char *src = $quote(multi
    line);

    $comment(
        puts("never compiled");
    );

    static $tls(int) thread_local_integer = 1;

    // etc...
}
*/
