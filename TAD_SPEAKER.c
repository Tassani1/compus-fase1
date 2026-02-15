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

/*=====================================================
  DEFINES GENERALS I CONSTANTS DE TEMPS / FREQÜÈNCIES
=====================================================*/

/* Duració del tic del mòdul TI en ms (assumit) */
#define TICK_MS                2

/* Conversió ms -> tics del TI */
#define MS_TO_TICS(ms)         ((ms) / TICK_MS)

/* Durades en mil·lisegons */
#define ACUTE_DURATION_MS      200     /* so agut curt */
#define ALARM_DURATION_MS      10000   /* alarma 10 s */
#define PRESSURE_TOTAL_MS      120000  /* 2 minuts */
#define PRESSURE_PHASE1_MS     105000  /* 1:45 = 105 s */
#define PRESSURE_PHASE2_MS     15000   /* 15 s finals */

#define PRESSURE_PERIOD1_MS    1000    /* bip cada 1 s */
#define PRESSURE_PERIOD2_MS    500     /* bip cada 500 ms */
#define SOUND_PULSE_MS         100     /* durada del bip (so) dins el període */

/* Freqüències (controlades per semiperíode de square wave) en ms */
#define ACUTE_SEMIPERIOD_MS    2       /* so agut (freq més alta) */
#define ALARM_SEMIPERIOD_MS    4       /* so d'alarma */
#define GRAVE_SEMIPERIOD_MS    8       /* so greu per pressió */

/* Mateixes constants en tics del TI */
#define ACUTE_DURATION_TICS        MS_TO_TICS(ACUTE_DURATION_MS)
#define ALARM_DURATION_TICS        MS_TO_TICS(ALARM_DURATION_MS)
#define PRESSURE_TOTAL_TICS        MS_TO_TICS(PRESSURE_TOTAL_MS)
#define PRESSURE_PHASE1_TICS       MS_TO_TICS(PRESSURE_PHASE1_MS)

#define PRESSURE_PERIOD1_TICS      MS_TO_TICS(PRESSURE_PERIOD1_MS)
#define PRESSURE_PERIOD2_TICS      MS_TO_TICS(PRESSURE_PERIOD2_MS)
#define SOUND_PULSE_TICS           MS_TO_TICS(SOUND_PULSE_MS)

#define ACUTE_SEMIPERIOD_TICS      MS_TO_TICS(ACUTE_SEMIPERIOD_MS)
#define ALARM_SEMIPERIOD_TICS      MS_TO_TICS(ALARM_SEMIPERIOD_MS)
#define GRAVE_SEMIPERIOD_TICS      MS_TO_TICS(GRAVE_SEMIPERIOD_MS)

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
    TI_ResetTics(TimerDuracio);
    TI_ResetTics(TimerPulse);
    TI_ResetTics(TimerTone);
}

/* Motor privat, cridat des del scheduler cooperatiu */
void SPE_Motor(void) {
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
            tDuracio = TI_GetTics(TimerDuracio);
            if (tDuracio >= ACUTE_DURATION_TICS) {
                SPE_StopInternal();
                Estat = 0;
                break;
            }

            /* Generació del to: square wave */
            tTone = TI_GetTics(TimerTone);
            if (tTone >= ToneSemiperiodTics) {
                LATDbits.LATD3 ^= 1;          /* toggle pin */
                TI_ResetTics(TimerTone);
            }
            break;

        /*------------------------------------
          Estat 2: So d'alarma continu (10 s)
        ------------------------------------*/
        case 2:
            tDuracio = TI_GetTics(TimerDuracio);
            if (tDuracio >= ALARM_DURATION_TICS) {
                SPE_StopInternal();
                Estat = 0;
                break;
            }

            tTone = TI_GetTics(TimerTone);
            if (tTone >= ToneSemiperiodTics) {
                LATDbits.LATD3 ^= 1;
                TI_ResetTics(TimerTone);
            }
            break;

        /*------------------------------------
          Estat 3: So de pressió (2 minuts)
          0–1:45  -> bip greu cada 1 s
          1:45–2:00 -> mateix bip cada 500 ms
        ------------------------------------*/
        case 3:
            tDuracio = TI_GetTics(TimerDuracio);
            if (tDuracio >= PRESSURE_TOTAL_TICS) {
                SPE_StopInternal();
                Estat = 0;
                break;
            }

            /* Determinar el període actual segons la fase */
            if (tDuracio < PRESSURE_PHASE1_TICS) {
                periodActualTics = PRESSURE_PERIOD1_TICS;   /* 1 s */
            } else {
                periodActualTics = PRESSURE_PERIOD2_TICS;   /* 500 ms */
            }

            tPulse = TI_GetTics(TimerPulse);

            /* Dins del període, durant SOUND_PULSE_TICS fem sonar el to greu */
            if (tPulse < SOUND_PULSE_TICS) {
                /* Beep actiu: generem square wave */
                tTone = TI_GetTics(TimerTone);
                if (tTone >= ToneSemiperiodTics) {
                    LATDbits.LATD3 ^= 1;
                    TI_ResetTics(TimerTone);
                }
            } else {
                /* Fora del pulse, assegurem el pin a 0 (silenci) */
                LATDbits.LATD3 = 0;
            }

            /* Si ja hem superat el període, recomencem el següent bip */
            if (tPulse >= periodActualTics) {
                TI_ResetTics(TimerPulse);
                TI_ResetTics(TimerTone);
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

void Init_Speaker(void) {
    TRISDbits.TRISD3 = 0;    /* RD3 com a sortida */
    LATDbits.LATD3 = 0;

    /* Inicialitzar timers del TI */
    TI_NewTimer(&TimerDuracio);
    TI_NewTimer(&TimerPulse);
    TI_NewTimer(&TimerTone);

    Estat = 0;
    SPE_StopInternal();
}

void SPE_StopSound(void) {
    SPE_StopInternal();
    Estat = 0;
}

/* So agut i curt (per portes, confirmacions, etc.) */
void SPE_PlayAcuteSound(void) {
    SPE_StopSound();                      /* cancel·la so anterior */
    ToneSemiperiodTics = ACUTE_SEMIPERIOD_TICS;

    TI_ResetTics(TimerDuracio);
    TI_ResetTics(TimerTone);

    Estat = 1;
}

/* So d'alarma continu durant 10 segons */
void SPE_PlayAlarmSound(void) {
    SPE_StopSound();
    ToneSemiperiodTics = ALARM_SEMIPERIOD_TICS;

    TI_ResetTics(TimerDuracio);
    TI_ResetTics(TimerTone);

    Estat = 2;
}

/* So de pressió durant 2 minuts:
   - 0–1:45  -> bip greu cada 1 s
   - 1:45–2:00 -> bip greu cada 500 ms
*/
void SPE_PlayPressureSound(void) {
    SPE_StopSound();
    ToneSemiperiodTics = GRAVE_SEMIPERIOD_TICS;

    TI_ResetTics(TimerDuracio);   /* compta global 0–2 min */
    TI_ResetTics(TimerPulse);     /* compta períodes (1 s / 500 ms) */
    TI_ResetTics(TimerTone);      /* square wave dins del bip */

    Estat = 3;
}
