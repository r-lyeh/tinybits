// Tiny todo() static assert macro. based on code by https://github.com/andyw8/do_by
// - rlyeh, public domain | wtrmrkrlyeh

#if defined(_MSC_VER) && _MSC_VER <= 1800
#define TODO(...) // constexpr wont work :(
#else
struct TODO {
    template<unsigned N>
    static constexpr unsigned DATE( const char (&DT)[N] ) {
        return /*D*/ ( DT[4] == ' ' ? 0 : (DT[4] - '0') * 10 ) + (DT[5] - '0') +
               /*M*/ ( DT[2] == 'n' ? 1
                     : DT[2] == 'b' ? 2
                     : DT[2] == 'r' && DT[0] == 'M' ? 3
                     : DT[2] == 'r' && DT[0] != 'M' ? 4
                     : DT[2] == 'y' ? 5
                     : DT[2] == 'n' ? 6
                     : DT[2] == 'l' ? 7
                     : DT[2] == 'g' ? 8
                     : DT[2] == 'p' ? 9
                     : DT[2] == 't' ? 10
                     : DT[2] == 'v' ? 11 : 12 ) * 100 +
                /*Y*/ (DT[7] - '0') * 1e7 + (DT[8] - '0') * 1e6 + (DT[9] - '0') * 1e5 + (DT[10] - '0') * 1e4;
    }
    template<unsigned N>
    static constexpr bool CHECK( const char (&DT)[N] ) {
        return DATE(__DATE__) < DATE(DT);
    }
#   define TODO(DT) \
        static_assert( TODO::CHECK(DT), "TODO date expired!" )
};
#endif

/*
int main() {
    TODO( "Sep 26 2015" ); // compile error here if current date >= Sep 26 2015
    TODO( "May 19 2014" ); // compile error here if current date >= May 19 2014
    TODO( "Aug 11 2011" ); // compile error here if current date >= Aug 11 2011
    return 0;
}
*/
