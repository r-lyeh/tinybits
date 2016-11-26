// Tiny digital pulses/signals. rlyeh, public domain | wtrmrkrlyeh
#include <stdio.h>

void pulse( int *state ) {
    switch( state[1] * 2 + state[0] ) {
        default:
        break; case 0: puts("pulse OFF");
        break; case 1: puts("pulse UP");
        break; case 2: puts("pulse DOWN");
        break; case 3: puts("pulse ON");
    }
    state[1] = state[0];
}

int main() {
    int state[2] = {0};

    *state = 0;
    pulse(state);

    *state = 1;
    pulse(state);

    *state = 1;
    pulse(state);

    *state = 0;
    pulse(state);

    *state = 0;
    pulse(state);
}
