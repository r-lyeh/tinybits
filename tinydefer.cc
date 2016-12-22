// tinydefer, Go style
// - rlyeh, public domain.

struct defer {
    void (*fn)();
    ~defer() { fn(); }
};

#define MERGE_(a,b)  a##b
#define LABEL_(a)    MERGE_(unique_name_, a)
#define UNIQUE_NAME  LABEL_(__LINE__)
#define defer        defer UNIQUE_NAME; UNIQUE_NAME.fn = [&]

#ifdef TINYDEFER_MAIN
#include <stdio.h>
void TINYDEFER_MAIN() {
    puts("1");

    defer {
        puts("2");
        puts("3");
    };

    defer {
        puts("4");
        puts("5");
    };

    puts("6");
}
#endif
