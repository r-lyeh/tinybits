// Tiny todo() static assert macro. based on code by https://github.com/andyw8/do_by
// - rlyeh, public domain | wtrmrkrlyeh

#pragma once
#include <stdio.h>

static inline int TODO(const char *DT) { return
       /*D*/ ( DT[4] == ' ' ? 0 : (DT[4] - '0') * 10 ) + (DT[5] - '0') +
       /*M*/ ( DT[2] == 'n' ? 1
             : DT[2] == 'b' ? 2
             : DT[2] == 'r' && DT[0] == 'M' ? 3
             : DT[2] == 'r' && DT[0] != 'M' ? 4
             : DT[2] == 'y' ? 5
             : DT[2] == 'n' ? 6
             : DT[2] == 'l' ? 7
             : DT[2] == 'g' ? 8
             : DT[2] == 'p' ? 9
             : DT[2] == 't' ? 10
             : DT[2] == 'v' ? 11 : 12 ) * 100 +
       /*Y*/ (DT[7] - '0') * 1e7 + (DT[8] - '0') * 1e6 + (DT[9] - '0') * 1e5 + (DT[10] - '0') * 1e4;
}

#define TODO(DT) do { static int ever = 0; if(!ever) { ever = 1; if( TODO(__DATE__) >= TODO(DT) ) fprintf(stderr, "TODO date expired! %s\n", DT); } } while(0)

/*
int main() {
    TODO( "Aug 11 2011: Finish prototype. Warn if current date >= Aug 11 2011" );
    TODO( "May 19 2014: Finish gameplay. Warn if current date >= May 19 2014" );
    TODO( "Sep 26 2015: Finish QA. Warn if current date >= Sep 26 2015" );
    return 0;
}
*/
