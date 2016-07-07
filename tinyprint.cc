// Tiny printer. Original code by Evan Wallace. rlyeh, public domain | wtrmrkrlyeh
#include <iostream>

struct print {
	const char *sep = "";
	~print() { std::cout << std::endl; }

	template<typename T> print& operator,( const T &t ) {
		return std::cout << sep << t, sep = " ", *this;
	}
};

#define print print(),


int main() {
   print "hello", 123, "world";
   print "yeah";
}
