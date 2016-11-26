// tiny C++ string utilities
// - rlyeh, public domain | wtrmrkrlyeh

// @toadd: pad, left, right, center, triml, trimr, trim, [-1]

#include <deque>
#include <string>

std::deque< std::string > tokenize( const std::string &self, const char *delimiters ) {
    char map[256] = {};
    while( *delimiters++ ) map[ delimiters[-1] ] = '\1';
    std::deque< std::string > tokens(1);
    for( auto &ch : self ) {
        /**/ if( !map[(unsigned) ch]  ) tokens.back().push_back( ch );
        else if( tokens.back().size() ) tokens.push_back( std::string() );
    }
    while( tokens.size() && !tokens.back().size() ) tokens.pop_back();
    return tokens;
}

std::deque< std::string > split( const std::string &self, const std::string &delimiters ) {
    std::string str;
    std::deque< std::string > tokens;
    for( auto &ch : self ) {
        if( delimiters.find_first_of( ch ) != std::string::npos ) {
            if( str.size() ) tokens.push_back( str ), str = "";
            tokens.push_back( std::string() + ch );
        } else str += ch;
    }
    return str.empty() ? tokens : ( tokens.push_back( str ), tokens );
}

std::string left_of( const std::string &substring, const std::string &self ) {
    std::string::size_type pos = self.find( substring );
    return pos == std::string::npos ? self : self.substr(0, pos);
}

std::string right_of( const std::string &substring, const std::string &self ) {
    std::string::size_type pos = self.find( substring );
    return pos == std::string::npos ? self : self.substr(pos + substring.size() );
}

std::string replace_one( const std::string &self, const std::string &target, const std::string &replacement ) {
    std::string str = self;
    auto found = str.find(target);
    return found == std::string::npos ? str : (str.replace(found, target.length(), replacement), str);
}

std::string replace_all( const std::string &self, const std::string &target, const std::string &replacement ) {
    size_t found = 0;
    std::string s = self;
    while( ( found = s.find( target, found ) ) != std::string::npos ) {
        s.replace( found, target.length(), replacement );
        found += replacement.length();
    }
    return s;
}

#include <assert.h>

int main() {
    assert( tokenize("a/b/c/\\d\\e,f,g,", "/\\,") == (std::deque<std::string> { "a", "b", "c", "d", "e", "f", "g" }) );
    assert( split("a/b/c/\\d\\e,f,g,", "/\\,") == (std::deque<std::string> { "a", "/", "b", "/", "c", "/", "\\", "d", "\\", "e", ",", "f", ",", "g", "," }) );
    assert( left_of("beginning", "in the beginning") == "in the " );
    assert( right_of("in the ", "in the beginning") == "beginning" );
    assert( replace_all( "0cad0", "0", "abra" ) == "abracadabra" );
    assert( replace_one( "0cad0", "0", "abra" ) == "abracad0" );
}
