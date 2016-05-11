// Tiny FSM. rlyeh, public domain

#include <stdio.h>

enum {
    IDLE,
    WALKING,
    RUNNING,
};

void fsm( int *state ) {
#   define is(a)   (((a)<<16)|(a))
#   define on(a,b) (((b)<<16)|(a))
    switch( on(state[1],state[0]) ) {
        default:
        break; case is(IDLE):            puts("idle");
        break; case on(IDLE,WALKING):    puts("idle --> walking");
        break; case on(IDLE,RUNNING):    puts("idle --> running");

        break; case is(WALKING):         puts("walking");
        break; case on(WALKING,IDLE):    puts("walking --> idle");
        break; case on(WALKING,RUNNING): puts("walking --> running");

        break; case is(RUNNING):         puts("running");
        break; case on(RUNNING,IDLE):    puts("running --> idle");
        break; case on(RUNNING,WALKING): puts("running --> walking");
    }
    state[1] = state[0];
}

/* or:
#   define fsm(state)  switch( ((state[0])<<16)|(state[1]) )
#   define is(a)       break; case (((a)<<16)|(a))
#   define on(a,b)     break; case (((b)<<16)|(a))
    fsm(state) {
        is(IDLE):            puts("idle");
        on(IDLE,WALKING):    puts("idle --> walking");
        on(IDLE,RUNNING):    puts("idle --> running");
    ...
    }
*/


int main() {
    int state[2] = {0};

    *state = IDLE;
    fsm(state);

    *state = WALKING;
    fsm(state);

    *state = WALKING;
    fsm(state);

    *state = RUNNING;
    fsm(state);

    *state = IDLE;
    fsm(state);
}
