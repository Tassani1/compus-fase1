#include <stdio.h>

#include "xc.h"
#include "TAD_TIMER.h"
#include "TAD_INTENSITY.h"

int main(void) {
    unsigned char timerHandle;

    TI_Init();
    if (!TI_NewTimer(&timerHandle)) {
        printf("No s'ha pogut crear el timer.\n");
        return 1;
    }

    TI_ResetTics(timerHandle);
    Init_Intensity();
    INT_start();

    for (unsigned long tick = 0; tick < 2000; tick++) {
        RSI_Timer0();
        INT_motor();

        if (tick % 200 == 0) {
            printf("tick=%lu tics=%lu led=%u\n",
                   tick,
                   TI_GetTics(timerHandle),
                   (unsigned int)LATDbits.LATD0);
        }
    }

    printf("final tics=%lu led=%u\n",
           TI_GetTics(timerHandle),
           (unsigned int)LATDbits.LATD0);

    return 0;
}
