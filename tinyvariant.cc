// Tiny variant class. rlyeh, public domain | wtrmrkrlyeh
#include <string>
#include <functional>

struct var {
    int type;
    union {
        int integer;
        double real;
        std::string *string;
        std::function<void()> *callback;
    };

    var() : type(0), integer(0)
    {}

    var( const int &i ) : type(0), integer(i)
    {}

    var( const double &r ) : type(1), real(r)
    {}

    var( const std::string &r ) : type(2), string( new std::string(r) )
    {}

    template<unsigned N>
    var( const char (&s)[N]) : type(2), string( new std::string(s) )
    {}

    template<typename T>
    var( const T &fn ) : type(3), callback( new std::function<void()>(fn) )
    {}

    var( const var &other ) {
        operator=( other );
    }

    ~var() {
        cleanup();
    }

    void cleanup() {
        /**/ if( type == 3 ) delete callback, callback = 0;
        else if( type == 2 ) delete string, string = 0;
        type = 0;
    }

    var &operator=( const var &other ) {
        if( &other != this ) {
            cleanup();
            type = other.type;
            /**/ if( type == 0 ) integer = other.integer;
            else if( type == 1 ) real = other.real;
            else if( type == 2 ) string = new std::string( *other.string );
            else if( type == 3 ) callback = new std::function<void()>( *other.callback );
        }
        return *this;
    }

    void operator()() const {
        if( type == 3 ) if( *callback ) (*callback)();
    }

    template<typename ostream>
    inline friend ostream &operator <<( ostream &os, const var &self ) {
        /**/ if( self.type == 0 ) return os << self.integer, os;
        else if( self.type == 1 ) return os << self.real, os;
        else if( self.type == 2 ) return os << *self.string, os;
        else if( self.type == 3 ) return os << '[' << self.callback << ']', os;
        return os;
    }
};

template<typename T> inline bool is(const var &v) { return false; }
template<>           inline bool is<int>(const var &v) { return v.type == 0; }
template<>           inline bool is<double>(const var &v) { return v.type == 1; }
template<>           inline bool is<std::string>(const var &v) { return v.type == 2; }
template<>           inline bool is<std::function<void()>>(const var &v) { return v.type == 3; }

template<typename T> inline const T& cast(const var &v) { static T t; return t = T(); }
template<>           inline const int& cast<int>(const var &v) { return v.integer; }
template<>           inline const double& cast<double>(const var &v) { return v.real; }
template<>           inline const std::string& cast<std::string>(const var &v) { return *v.string; }
template<>           inline const std::function<void()>& cast<std::function<void()>>(const var &v) { return *v.callback; }

/*
#include <iostream>
#include <assert.h>
int main() {
    std::cout << "sizeof(var)=" << sizeof(var) << std::endl;

    var digit = 42, other = "hello world";

    std::cout << digit << std::endl; assert( is<int>(digit) );
    std::cout << other << std::endl; assert( is<std::string>(other) );

    other = digit;
    std::cout << other << std::endl; assert( is<int>(other) );

    other = []{ std::cout << "callback!" << std::endl; };
    std::cout << other << std::endl;
    other();
}
*/
