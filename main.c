/*
 * File:   main.c
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:33
 */

#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_SERIAL.h"
#include "TAD_TECLAT.h"
#include "TAD_CONTROLLER.h"
#include "TAD_LEDS.h"
#include "TAD_INTENSITY.h"
#include "TAD_HALL.h"
#include "TAD_SPEAKER.h"
#include "TAD_EXIT_REQUEST.h"

#pragma config OSC = HS //INTIO2
#pragma config PBADEN = DIG
#pragma config MCLRE = OFF
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

void __interrupt() RSI_High(void){
	RSI_Timer0();
}

void main(void) { 
    TRISDbits.TRISD3 = 0;
    LATDbits.LATD3 = 0;

    speaker_init();
    timer_init();
    serial_init();
    teclat_init();
    intensity_init();
    leds_init();
    hall_init();
    controller_init();
    exitRequest_init();
    
//    TRISAbits.TRISA4 = 0;
//    LATAbits.LATA4 = 0;
    
    while(1) {
        controller_motor();
        //intensity_motor();
        teclat_motor();
        serial_motor();
        speaker_motor();
        exitRequest_motor();
        
        //LATDbits.LATD0 = 1;
    }
    
    return;
}
