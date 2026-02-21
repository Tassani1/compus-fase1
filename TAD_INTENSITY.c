/*
 * File:   TAD_INTENSITY.c
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#include "TAD_INTENSITY.h"
#include <xc.h>
#include "pic18f4321.h"
#include "TAD_TIMER.h"

static unsigned long tick_counter = 0;        // Comptador global de ticks (2ms)
static unsigned long step_counter = 0;        // Comptador per saber quan pujar %
static unsigned char intensity_percent = 0;  // 0..100
static unsigned char pwm_counter = 0;        // 0..99
static unsigned char estat = 0;
static unsigned char timerGlobal;
static unsigned char timerSegment;

void intensity_init(void) {
    TRISDbits.TRISD0 = 0; // Configura RD0 como salida para el LED de intensidad
    LATDbits.LATD0 = 0;
    timer_newTimer(&timerGlobal);
    timer_newTimer(&timerSegment);
}

void intensity_start(void) {
    tick_counter = 0;
    step_counter = 0;
    pwm_counter = 0;
    intensity_percent = 0;
    estat = 1; // Comen�a a incrementar la intensitat
    timer_resetTics(timerGlobal);
    timer_resetTics(timerSegment);
}

void intensity_stop(void) {
    estat = 0;
    intensity_percent = 0;
    pwm_counter = 0;
    LATDbits.LATD0 = 0;   // Apagar LED
}

void intensity_motor(void) {
    switch (estat) {
        case 0: 
            break;
        case 1: // Control de la intensitat i PWM

            // --- Control del temps total (2 minuts) ---
            tick_counter = timer_getTics(timerGlobal);
            step_counter = timer_getTics(timerSegment);

            // Cada 600 ticks (1,2 s) incrementem 1%
            if (step_counter >= 600){
                timer_resetTics(timerSegment);

                if (intensity_percent < 100) {
                    intensity_percent++;
                }
            }

            // --- PWM per software ---
            pwm_counter++;
            if (pwm_counter >= 100) {
                pwm_counter = 0;
            }
            if (pwm_counter < intensity_percent) {
                LATDbits.LATD0 = 1;
            } else {
                LATDbits.LATD0 = 0;
            }
            if (tick_counter >= 100 * 600) { // Si han passat 2 minuts, aturem
                intensity_stop();
            }
            break;
    }
}