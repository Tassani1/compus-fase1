/*
 * File:   TAD_STATE_OK.c
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#include "TAD_LEDS.h"
#include <xc.h>
#include "pic18f4321.h"

#define LED_ENCES 1 
#define LED_APAGAT 0

// Alarma   LATD1
// State OK LATD2

void leds_init (void) {
    // Activem les sortides de PORT D[2] i PORT D[1] de sortida, i els posem a 0.
    TRISDbits.TRISD1=0;
    TRISDbits.TRISD2=0;
    LATDbits.LATD1=LED_APAGAT;
    LATDbits.LATD2=LED_APAGAT;
}

void leds_encenLed (char Led) {
    if (Led==LED_STATE_OK) {
        LATDbits.LATD2=LED_ENCES;
    } else if (Led==LED_STATE_ALARM) {
        LATDbits.LATD1=LED_ENCES;
    }
}

void leds_apagaLed (char Led) {
    if (Led==LED_STATE_OK) {
        LATDbits.LATD2=LED_APAGAT;
    } else if (Led==LED_STATE_ALARM) {
        LATDbits.LATD1=LED_APAGAT;
    }
}