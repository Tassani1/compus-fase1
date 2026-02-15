/*
 * File:   TAD_CONTROLLER.c
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#include "TAD_CONTROLLER.h"
#include <xc.h>
#include "pic18f4321.h"
#include "TAD_TIMER.h"
#include "TAD_SERIAL.h"
#include "TAD_SPEAKER.h"
#include "TAD_INTENSITY.h"
#include "TAD_STATE_OK.h"
#include "TAD_STATE_ALARM.h"
#include "TAD_EXIT_REQUEST.h"
#include "TAD_LEDS.h"
//TODO: incloure .h del HALL

static unsigned char Estat;
static unsigned char timerController;

void Init_Controller(void){
    TI_NewTimer(&timerController);
}

void CO_HallDetected(void) {}

void CO_NewKeyPressed(unsigned char key) {}

void CO_ExitRequested(void) {}

void CO_motor(){
    unsigned char hallActivat = 0;
    switch(Estat){
        case 0: 
            //TODO: Funció del serial que printi el missatge de bevinguda
            LEDS_EncenLed(LED_STATE_OK);
            TI_ResetTics(timerController);
            Estat = 1;
            
            break;
        case 1:
            //TODO: posar un if per si es detecta el HALL
                //dintre del if
                //TODO: Funcio del serial que printi el missatge: >LSBank-Open exterior door
                //espera de 2 segons
                hallActivat = 1;

                if(hallActivat){
                    if(TI_GetTics(timerController)>= 1000){
                    //TODO: Funcio del serial que printi el missatge: >LSBank-close/closed exterior door
                    SPE_PlayAcuteSound();
                    //TODO: Funcio del serial que printi el missatge: >LSBank_EnterPIN:
                    Estat = 2;
                    hallActivat = 0;
                    }
                }
        case 2: 
                
            
                

                    
            


    }
}

