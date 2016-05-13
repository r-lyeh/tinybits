// Tiny session logger. rlyeh, public domain
#include <stdio.h>

#ifdef SHIPPING
struct logger {
    logger() {
        fclose(stdout);
    }
};
#else
struct logger {
    logger() {
#       if   defined(PSVITA)
        freopen("host0://log_vita.txt", "a+t", stdout);
#       elif defined(PS3)
        freopen("/app_home/log_ps3.txt", "a+t", stdout);
#       elif defined(PS4)
        freopen("/hostapp/log_ps4.txt", "a+t", stdout);
#       else
        freopen("log_desktop.txt", "a+t", stdout);
#       endif
        // Flush automatically every 16 KiB from now
        setvbuf(stdout, NULL, _IOFBF, 16 * 1024);
        // Header
        puts(";; New session");
        fflush(stdout);
    }
    ~logger() {
        fflush(stdout);
    }
};
#endif

int main() {
    logger resident;
    puts("hello world");
}
