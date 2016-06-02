// tiny wildcard/pattern matching. Based on anonymous souce code.
// - rlyeh. public domain.

static int match( const char *pattern, const char *str ) {
    if( *pattern=='\0' ) return !*str;
    if( *pattern=='*' )  return match(pattern+1, str) || (*str && match(pattern, str+1));
    if( *pattern=='?' )  return *str && (*str != '.') && match(pattern+1, str+1);
    return (*str == *pattern) && match(pattern+1, str+1);
}

#include <stdio.h>

int main() {
    printf("%s\n", match("abc", "abc") ? "match!" : "not found" );
    printf("%s\n", match("abc*", "abc") ? "match!" : "not found" );
    printf("%s\n", match("*bc", "abc") ? "match!" : "not found" );
    printf("%s\n", match("*bc*", "abc") ? "match!" : "not found" );
    printf("%s\n", match("*b?d*", "abcdef") ? "match!" : "not found" );
}
