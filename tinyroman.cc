// Tiny integer to roman numerals converter (roughly tested). rlyeh, public domain | wtrmrkrlyeh
#include <map>
#include <string>

std::string romanize( int i ) {
    static std::string table[] = {
        "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX",
        "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC",
        "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM",
        "M", "MM", "MMM", "MMMM",
    };
    std::string out;
    for( int base = 0; i > 0; base++, i /= 10 ) {
        int mod = i % 10;
        if( mod > 0 ) {
            out = table[(mod - 1) + base * 9] + out;
        }
    }
    return out;
}

#include <cassert>
int main() {
    assert( romanize(0) == "" );
    assert( romanize(10) == "X" );
    assert( romanize(1990) == "MCMXC" );
    assert( romanize(2008) == "MMVIII" );
    assert( romanize(99) == "XCIX" );
    assert( romanize(47) == "XLVII" );
}
