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
static char intentsRestants = 3;
static unsigned char hallActivat = 0;
static unsigned char PIN[8];
static unsigned char iPIN = 0;
static unsigned char PIN_CORRECTE[] = "1511MTV"
static unsigned char esCorrecte = 1;

void Init_Controller(void){
    TI_NewTimer(&timerController);
}

void CO_HallDetected(void) {}



void CO_NewKeyPressed(unsigned char key) {
    //DDMMXXX + /0
    if(iPIN <8){
        PIN[iPIN++] = key;
    }   

}

void CO_ExitRequested(void) {}

void CO_motor(){
   

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
                //TODO: Funcio del serial que printi el missatge: > LSBank - Open exterior door
                //espera de 2 segons
                hallActivat = 1;

                if(hallActivat){
                    if(TI_GetTics(timerController)>= 1000){
                        //TODO: Funcio del serial que printi el missatge: > LSBank - close/closed exterior door
                        SPE_PlayAcuteSound();
                        //TODO: Funcio del serial que printi el missatge: > LSBank - Enter PIN:
                        Estat = 2;
                        hallActivat = 0;
                        TI_ResetTics(timerController);
                    }
                }
        case 2: 
                
                //comprovar si no han passat 2 minuts
                if(TI_GetTics(timerController)<= 60000){
                    esCorrecte = 1;
                    SPE_PlayPressureSound();
                    INT_start();
                    if(iPIN == 7){
                        PIN[8] = '\0';
                        for(unsigned char i = 0; i<8 ; i++){
                            if(PIN[i] != PIN_CORRECTE[i]){
                                esCorrecte = 0;
                                break;
                            }                            
                        }
                    }
                    iPIN = 0;
                    if (!esCorrecte){
                        if(--intentsRestants == 0){
                            //ALARMA
                            SPE_PlayAlarmSound();
                            //TODO: Serial posasr: > LSBank - Thief Intercepted 
                            LEDS_EncenLed (LED_STATE_ALARM);
                            LEDS_ApagaLed (LED_STATE_OK);
                            TI_ResetTics(timerController);
                            INT_stop();
                            Estat = 3;

                        
                        } else{

                            //posar > LSBank - Permision Denied
                            //posar > LSBank - Enter PIN
                        }
                        

                        
                    }

                }
                else{
                    Estat = 3;
                }
        //estat alarma
        case 3: 
                
                
            
                

                    
            


    }
}

