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

/* Constants en tics sabent el tick és de 2 ms */
#define ACUTE_DURATION      100     /* so agut curt */
#define ALARM_DURATION      5000    /* alarma 10 s */
#define PRESSURE_TOTAL      60000   /* 2 minuts */
#define PRESSURE_PHASE1     52500   /* 1:45 = 105 s */

#define PRESSURE_PERIOD1    500     /* 1 s */
#define PRESSURE_PERIOD2    250     /* 500 ms */
#define SOUND_PULSE         50      /* 100 ms */

#define ACUTE_SEMIPERIOD    1       /* 2 ms */
#define ALARM_SEMIPERIOD    2       /* 4 ms */
#define GRAVE_SEMIPERIOD    4       /* 8 ms */

/*=====================================================
  VARIABLES PRIVADES
=====================================================*/

/* Estat del motor del TAD_SPEAKER */
static unsigned char Estat = 0;

/* Timers del mòdul TI */
static unsigned char TimerDuracio;   /* duració global del so actual */
static unsigned char TimerPulse;     /* es el temps de silenci entre bips (mode pressió) */
static unsigned char TimerTone;      /* serveix per saber la frequencia del to del so */

/* Paràmetres del so actual */
static unsigned int ToneSemiperiodTics = 0;   /* semiperíode en tics TI */

/*=====================================================
  FUNCIONS PRIVADES
=====================================================*/

static void SPE_StopInternal(void) {
    LATDbits.LATD3 = 0;
    timer_resetTics(TimerDuracio);
    timer_resetTics(TimerPulse);
    timer_resetTics(TimerTone);
}

/* Motor privat, cridat des del scheduler cooperatiu */
void speaker_motor(void) {
    unsigned int tDuracio;
    unsigned int tPulse;
    unsigned int tTone;
    unsigned int periodActualTics;

    switch(Estat) {

        /*------------------------------------
          Estat 0: Idle (sense so)
        ------------------------------------*/
        case 0:
            /* No fem res */
            break;

        /*------------------------------------
          Estat 1: So agut curt
        ------------------------------------*/
        case 1:
            tDuracio = timer_getTics(TimerDuracio);
            if (tDuracio >= ACUTE_DURATION) {
                SPE_StopInternal();
                Estat = 0;
                break;
            }

            /* Generació del to: square wave */
            tTone = timer_getTics(TimerTone);
            if (tTone >= ToneSemiperiodTics) {
                LATDbits.LATD3 ^= 1;          /* toggle pin */
                timer_resetTics(TimerTone);
            }
            break;

        /*------------------------------------
          Estat 2: So d'alarma continu (10 s)
        ------------------------------------*/
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

        /*------------------------------------
          Estat 3: So de pressió (2 minuts)
          0–1:45  -> bip greu cada 1 s
          1:45–2:00 -> mateix bip cada 500 ms
        ------------------------------------*/
        case 3:
            tDuracio = timer_getTics(TimerDuracio);
            if (tDuracio >= PRESSURE_TOTAL) {
                SPE_StopInternal();
                Estat = 0;
                break;
            }

            /* Determinar el període actual segons la fase */
            if (tDuracio < PRESSURE_PHASE1) {
                periodActualTics = PRESSURE_PERIOD1;   /* 1 s */
            } else {
                periodActualTics = PRESSURE_PERIOD2;   /* 500 ms */
            }

            tPulse = timer_getTics(TimerPulse);

            /* Dins del període, durant SOUND_PULSE fem sonar el to greu */
            if (tPulse < SOUND_PULSE) {
                /* Beep actiu: generem square wave */
                tTone = timer_getTics(TimerTone);
                if (tTone >= ToneSemiperiodTics) {
                    LATDbits.LATD3 ^= 1;
                    timer_resetTics(TimerTone);
                }
            } else {
                /* Fora del pulse, assegurem el pin a 0 (silenci) */
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
            /* Estat desconegut: parem per seguretat */
            SPE_StopInternal();
            Estat = 0;
            break;
    }
}

/*=====================================================
  FUNCIONS PÚBLIQUES
=====================================================*/

void speaker_init(void) {
    TRISDbits.TRISD3 = 0;    /* RD3 com a sortida */
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

/* So agut i curt (per portes, confirmacions, etc.) */
void speaker_playAcuteSound(void) {
    speaker_stopSound();                      /* cancel·la so anterior */
    ToneSemiperiodTics = ACUTE_SEMIPERIOD;

    timer_resetTics(TimerDuracio);
    timer_resetTics(TimerTone);

    Estat = 1;
}

/* So d'alarma continu durant 10 segons */
void speaker_playAlarmSound(void) {
    speaker_stopSound();
    ToneSemiperiodTics = ALARM_SEMIPERIOD;

    timer_resetTics(TimerDuracio);
    timer_resetTics(TimerTone);

    Estat = 2;
}

/* So de pressió durant 2 minuts:
   - 0–1:45  -> bip greu cada 1 s
   - 1:45–2:00 -> bip greu cada 500 ms
*/
void speaker_playPressureSound(void) {
    speaker_stopSound();
    ToneSemiperiodTics = GRAVE_SEMIPERIOD;

    timer_resetTics(TimerDuracio);   /* compta global 0–2 min */
    timer_resetTics(TimerPulse);     /* compta períodes (1 s / 500 ms) */
    timer_resetTics(TimerTone);      /* square wave dins del bip */

    Estat = 3;
}
