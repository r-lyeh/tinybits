// Tiny unittest suite. rlyeh, public domain
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define suite(...) if(printf("------ " __VA_ARGS__),puts(""),1)
#define test(...)  (errno=0,++tst,err+=!(ok=!!(__VA_ARGS__))),printf("[%s] L%d %s (%s)\n",ok?" OK ":"FAIL",__LINE__,#__VA_ARGS__,strerror(errno))
unsigned tst=0,err=0,ok=atexit( []{ suite("summary"){ printf("[%s] %d tests = %d passed + %d errors\n",err?"FAIL":" OK ",tst,tst-err,err); }; });

int main() {
    /* orphan test */
    test(1<2);
    /* grouped tests */
    suite("grouped tests") {
        test(1<2);
        test(1<2);
    }
}
