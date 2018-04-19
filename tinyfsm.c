// Tiny FSM. rlyeh, public domain | wtrmrkrlyeh

#pragma once
#define with(st)        for(int i=1;i--;st[1]=st[0]) switch(((st[0])<<16)|(st[1]))
#define when(a)         break; case (((a)<<16)|(a))
#define transition(a,b) break; case (((b)<<16)|(a))
typedef int fsm[2];


#ifdef TINYFSM_DEMO
#include <stdio.h>

enum {
    IDLE,
    WALKING,
    RUNNING,
};

void update( fsm state ) {
    with(state) {
        when(IDLE):                  puts("idle");
        transition(IDLE,WALKING):    puts("idle --> walking");
        transition(IDLE,RUNNING):    puts("idle --> running");

        when(WALKING):               puts("walking");
        transition(WALKING,IDLE):    puts("walking --> idle");
        transition(WALKING,RUNNING): puts("walking --> running");

        when(RUNNING):               puts("running");
        transition(RUNNING,IDLE):    puts("running --> idle");
        transition(RUNNING,WALKING): puts("running --> walking");
    }
}

void TINYFSM_DEMO() {
    fsm state = {0};

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
