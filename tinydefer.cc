// tinydefer, Go style
// - rlyeh, public domain.

struct defer {
    void (*fn)();
    ~defer() { fn(); }
};

#define DEFER_MERGE_(a,b)  a##b
#define DEFER_LABEL_(a)    DEFER_MERGE_(unique_name_, a)
#define DEFER_UNIQUE_NAME  DEFER_LABEL_(__LINE__)
#define defer        defer DEFER_UNIQUE_NAME; DEFER_UNIQUE_NAME.fn = [&]

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
