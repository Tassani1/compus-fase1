/*
 * File:   TAD_SPEAKER.c
 * Author: Ari i Marc
 *
 * Created on 15 de febrero de 2026, 17:17
 */

#include "TAD_SPEAKER.h"
#include "TAD_TIMER.h"
#include <xc.h>
#include "pic18f4321.h"


#define ACUTE_DURATION      100     //so agut curt
#define ALARM_DURATION      5000    //alarma 10s
#define PRESSURE_TOTAL      60000   //2 mins
#define PRESSURE_PHASE1     52500   // 1:45

#define PRESSURE_PERIOD1    500     //1s
#define PRESSURE_PERIOD2    250     //500 ms
#define SOUND_PULSE         50      // 100 ms 

#define ACUTE_SEMIPERIOD    1       // 2 ms
#define ALARM_SEMIPERIOD    2       // 4 ms 
#define GRAVE_SEMIPERIOD    4       // 8 ms

static unsigned char Estat = 0;
static unsigned char TimerDuracio;   // duració global del so actual
static unsigned char TimerPulse;     // es el temps de silenci entre pitidos (mode pressió)
static unsigned char TimerTone;      // frequencia del to del so 

static unsigned int ToneSemiperiodTics = 0;


static void SPE_StopInternal(void) {
    LATDbits.LATD3 = 0;
    timer_resetTics(TimerDuracio);
    timer_resetTics(TimerPulse);
    timer_resetTics(TimerTone);
}
void speaker_init(void) {
    TRISDbits.TRISD3 = 0;
    LATDbits.LATD3 = 0;

    /* Inicialitzar timers del TI */
    timer_newTimer(&TimerDuracio);
    timer_newTimer(&TimerPulse);
    timer_newTimer(&TimerTone);

    Estat = 0;
    SPE_StopInternal();
}

void speaker_stopSound(void) {
    SPE_StopInternal();
    Estat = 0;
}


void speaker_playAcuteSound(void) {
    speaker_stopSound(); // cancel·la so anterior
    ToneSemiperiodTics = ACUTE_SEMIPERIOD;

    timer_resetTics(TimerDuracio);
    timer_resetTics(TimerTone);

    Estat = 1;
}

void speaker_playAlarmSound(void) {
    speaker_stopSound();
    ToneSemiperiodTics = ALARM_SEMIPERIOD;

    timer_resetTics(TimerDuracio);
    timer_resetTics(TimerTone);

    Estat = 2;
}


void speaker_playPressureSound(void) {
    speaker_stopSound();
    ToneSemiperiodTics = GRAVE_SEMIPERIOD;

    timer_resetTics(TimerDuracio);   //compta 2mins
    timer_resetTics(TimerPulse);     //compta (1 s / 500 ms)
    timer_resetTics(TimerTone);      

    Estat = 3;
}

void speaker_motor(void) {
    unsigned int tDuracio;
    unsigned int tPulse;
    unsigned int tTone;
    unsigned int periodActualTics;

    switch(Estat) {

      
        case 0:
            break;

        
          //AGUT
        case 1:
            tDuracio = timer_getTics(TimerDuracio);
            if (tDuracio >= ACUTE_DURATION) {
                SPE_StopInternal();
                Estat = 0;
                break;
            }

            tTone = timer_getTics(TimerTone);
            if (tTone >= ToneSemiperiodTics) {
                LATDbits.LATD3 ^= 1;
                timer_resetTics(TimerTone);
            }
            break;

        //ALARMA
        case 2:
            tDuracio = timer_getTics(TimerDuracio);
            if (tDuracio >= ALARM_DURATION) {
                SPE_StopInternal();
                Estat = 0;
                break;
            }

            tTone = timer_getTics(TimerTone);
            if (tTone >= ToneSemiperiodTics) {
                LATDbits.LATD3 ^= 1;
                timer_resetTics(TimerTone);
            }
            break;

        //PRESSIO (canviem el periode de pitidos al 1:45)
        case 3:
            tDuracio = timer_getTics(TimerDuracio);
            if (tDuracio >= PRESSURE_TOTAL) {
                SPE_StopInternal();
                Estat = 0;
                break;
            }

            // Determinem el període actual segons la fase
            if (tDuracio < PRESSURE_PHASE1) {
                periodActualTics = PRESSURE_PERIOD1;
            } else {
                periodActualTics = PRESSURE_PERIOD2;
            }

            tPulse = timer_getTics(TimerPulse);

            if (tPulse < SOUND_PULSE) {
                //temps de soroll
                tTone = timer_getTics(TimerTone);
                if (tTone >= ToneSemiperiodTics) {
                    LATDbits.LATD3 ^= 1;
                    timer_resetTics(TimerTone);
                }
            } else {
                //temps de silenci
                LATDbits.LATD3 = 0;
            }

            /* Si ja hem superat el període, recomencem el següent bip */
            if (tPulse >= periodActualTics) {
                timer_resetTics(TimerPulse);
                timer_resetTics(TimerTone);
                LATDbits.LATD3 = 0;
            }
            break;

        default:
            //aturar per seguretat
            SPE_StopInternal();
            Estat = 0;
            break;
    }
}




