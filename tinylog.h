// Tiny logging utilities. rlyeh, public domain
#pragma once
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#define TTY(ansi) ""
#else
#define TTY(ansi) ansi
#endif

#define TRACE(...) do { time_t t = time(0); printf("%s[TRACE %.8s]%s %s:%d ", TTY("\27[34m"), 11+ctime(&t), TTY("\27[0m"), __FILE__, __LINE__); printf(__VA_ARGS__); } while(0)
#define DEBUG(...) do { time_t t = time(0); printf("%s[DEBUG %.8s]%s %s:%d ", TTY("\27[36m"), 11+ctime(&t), TTY("\27[0m"), __FILE__, __LINE__); printf(__VA_ARGS__); } while(0)
#define INFO(...)  do { time_t t = time(0); printf("%s[INFO  %.8s]%s %s:%d ", TTY("\27[32m"), 11+ctime(&t), TTY("\27[0m"), __FILE__, __LINE__); printf(__VA_ARGS__); } while(0)
#define WARN(...)  do { time_t t = time(0); printf("%s[WARN  %.8s]%s %s:%d ", TTY("\27[33m"), 11+ctime(&t), TTY("\27[0m"), __FILE__, __LINE__); printf(__VA_ARGS__); } while(0)
#define ERROR(...) do { time_t t = time(0); printf("%s[ERROR %.8s]%s %s:%d ", TTY("\27[31m"), 11+ctime(&t), TTY("\27[0m"), __FILE__, __LINE__); printf(__VA_ARGS__); } while(0)
#define FATAL(...) do { time_t t = time(0); printf("%s[FATAL %.8s]%s %s:%d ", TTY("\27[35m"), 11+ctime(&t), TTY("\27[0m"), __FILE__, __LINE__); printf(__VA_ARGS__); } while(0)

/*
int main() {
    FATAL("Hello %d\n", 123);
    ERROR("Hello %d\n", 123);
    WARN("Hello %d\n", 123);
    INFO("Hello %d\n", 123);
    DEBUG("Hello %d\n", 123);
    TRACE("Hello %d\n", 123);
}
*/
