#include <stdint.h>
#include <stdio.h>

typedef void (*ontimer)(uint32_t timeoutCycles);
ontimer timer_callback = NULL;

void setTimerCallback(ontimer callback) {
    timer_callback = callback;
}
uint32_t incrementTimeoutCycles(uint32_t timeoutCycles) {
    if(timer_callback) {
        timer_callback(timeoutCycles);
    }
    return ++timeoutCycles;
}