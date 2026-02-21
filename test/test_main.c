#include <stdio.h>

#include "xc.h"
#include "TAD_TIMER.h"
#include "TAD_INTENSITY.h"

int main(void) {
    unsigned char timerHandle;

    TI_init();
    if (!timer_newTimer(&timerHandle)) {
        printf("No s'ha pogut crear el timer.\n");
        return 1;
    }

    timer_resetTics(timerHandle);
    intensity_init();
    INT_start();

    for (unsigned long tick = 0; tick < 2000; tick++) {
        RSI_Timer0();
        intensity_motor();

        if (tick % 200 == 0) {
            printf("tick=%lu tics=%lu led=%u\n",
                   tick,
                   timer_getTics(timerHandle),
                   (unsigned int)LATDbits.LATD0);
        }
    }

    printf("final tics=%lu led=%u\n",
           timer_getTics(timerHandle),
           (unsigned int)LATDbits.LATD0);

    return 0;
}
