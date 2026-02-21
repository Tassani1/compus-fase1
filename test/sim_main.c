#include <stdio.h>
#include "xc.h"
#include "TAD_TIMER.h"
#include "TAD_SERIAL.h"
#include "TAD_TECLAT.h"
#include "TAD_CONTROLLER.h"
#include "TAD_LEDS.h"
#include "TAD_INTENSITY.h"
#include "TAD_HALL.h"
#include "TAD_SPEAKER.h"

// Mock for interrupt
void RSI_High(void) {
    RSI_Timer0();
}

int main(void) {
    printf("[sim_main] Iniciant simulació main.c...\n");
    speaker_init();
    timer_init();
    serial_init();
    teclat_init();
    intensity_init();
    leds_init();
    hall_init();
    controller_init();

    unsigned long ticks = 0;
    while (ticks < 10000) { // Simula 1000 iteracions
        controller_motor();
        teclat_motor();
        serial_motor();
        speaker_motor();
        RSI_Timer0(); // Simula el timer
        if (ticks % 100 == 0) {
            printf("[sim_main] tick=%lu\n", ticks);
        }
        ticks++;
    }
    printf("[sim_main] Simulació acabada.\n");
    return 0;
}
