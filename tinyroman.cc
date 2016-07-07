// Tiny integer to roman numerals converter (roughly tested). rlyeh, public domain | wtrmrkrlyeh
#include <map>
#include <string>

std::string roman( int i ) {
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
    assert( roman(0) == "" );
    assert( roman(10) == "X" );
    assert( roman(1990) == "MCMXC" );
    assert( roman(2008) == "MMVIII" );
    assert( roman(99) == "XCIX" );
    assert( roman(47) == "XLVII" );
}
