/*
 * File:   TAD_EXIT_REQUEST.h
 * Author: Ari i Marc
 *
 * Created on 15 de febrero de 2026, 17:44
 */

#include "TAD_EXIT_REQUEST.h"
#include <xc.h>
#include "pic18f4321.h"
#include "TAD_TIMER.h"
#include "TAD_CONTROLLER.h"
#include "TAD_LEDS.h"

#define POLSADOR_PREMUT 1
static unsigned char Estat = 0;
static unsigned char Timer = 0;

void exitRequest_init(){
    TRISBbits.TRISB0 = 1;
    INTCON2bits.RBPU = 0;
    timer_newTimer(&Timer);
}

void exitRequest_motor(){
    switch(Estat){
        case 0:
            if(PORTBbits.RB0 == POLSADOR_PREMUT){
                timer_resetTics(Timer);
                Estat = 1;
                leds_encenLed(LED_STATE_ALARM);
            }
            break;
        case 1:
            
            if(timer_getTics(Timer)>=100){
                Estat = 2;
            }
            break;
        case 2:
           
            if(PORTBbits.RB0 != POLSADOR_PREMUT){
                timer_resetTics(Timer);
                
                Estat = 3;
                 
            }
            break;
        case 3: 
            
            if(timer_getTics(Timer)>=100){
                leds_apagaLed(LED_STATE_ALARM);
                controller_exitRequested();
                Estat = 0;
            }
            
    }

}



