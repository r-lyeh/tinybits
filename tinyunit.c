// Tiny unittest suite. rlyeh, public domain | wtrmrkrlyeh
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define suite(...) if(printf("------ " __VA_ARGS__),puts(""),1)
#define test(...)  (errno=0,++tst,err+=!(ok=!!(__VA_ARGS__))),printf("[%s] L%d %s (%s)\n",ok?" OK ":"FAIL",__LINE__,#__VA_ARGS__,strerror(errno))
static unsigned tst=0,err=0,ok=1; static void summary(void){ suite("summary"){ printf("[%s] %d tests = %d passed + %d errors\n",err?"FAIL":" OK ",tst,tst-err,err); }; }

int main() {
    atexit( summary );
    /* orphan test */
    test(1<2);
    /* grouped tests */
    suite("grouped tests %d/%d", 1,1) {
        test(1<2);
        test(1<2);
    }
    suite("grouped tests %d/%d", 1,2) {
        test(1<2);
        test(1<2);
    }
}
