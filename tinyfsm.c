// Tiny FSM. rlyeh, public domain | wtrmrkrlyeh

#   define fsm(st)     for(int i=1;i--;st[1]=st[0]) switch(((st[0])<<16)|(st[1]))
#   define when(a)     break; case (((a)<<16)|(a))
#   define during(a,b) break; case (((b)<<16)|(a))

#ifdef TINYFSM_DEMO
#include <stdio.h>

enum {
    IDLE,
    WALKING,
    RUNNING,
};

void update( int *state ) {
    fsm(state) {
        when(IDLE):              puts("idle");
        during(IDLE,WALKING):    puts("idle --> walking");
        during(IDLE,RUNNING):    puts("idle --> running");

        when(WALKING):           puts("walking");
        during(WALKING,IDLE):    puts("walking --> idle");
        during(WALKING,RUNNING): puts("walking --> running");

        when(RUNNING):           puts("running");
        during(RUNNING,IDLE):    puts("running --> idle");
        during(RUNNING,WALKING): puts("running --> walking");
    }
}

void TINYFSM_DEMO() {
    int state[2] = {0};

    *state = IDLE;
    update(state);

    *state = WALKING;
    update(state);

    *state = WALKING;
    update(state);

    *state = RUNNING;
    update(state);

    *state = IDLE;
    update(state);
}

#endif
