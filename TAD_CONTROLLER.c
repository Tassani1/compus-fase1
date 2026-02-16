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
static unsigned char PIN[8];
static unsigned char iPIN = 0;
static unsigned char esCorrecte = 1;
static unsigned char Static Alarma = 1;
static unsigned char PIN_CORRECTE = 0; 

static unsigned char PIN_CORRECTE[] = "1511MTV";
static unsigned char SOLICITUD_ACCEPTADA[] = "Yes";
static unsigned char SOLICITUD_DENEGADA[] = "No";

const char PORTA_EXTERIOR_OBERTA[] = "> LSBank - open exterior door";
const char PORTA_EXTERIOR_TANCADA[] = "> LSBank - close exterior door";

const char MISSATGE_ENTRA_PIN[] = "> LSBank - Enter PIN";

const char PERMIS_DENEGAT[] = "> LSBank - Permission Denied";

const char PORTA_INTERIOR_OBERTA[] = "> LSBank - open interior door";
const char PORTA_INTERIOR_TANCADA[] = "> LSBank - close interior door";

const char EXIT_REQUESTED[] = "> LSBank - Exit Requested";

const char DUES_PORTES_OBERTES[] = "> LSBank - Open both doors";
const char DUES_PORTES_TANCADES[] = "> LSBank - Close both doors";

const char LLADRE_INTERCEPTAT[] = "> LSBank - Thief Intercepted";
const char RESET_SISTEMA[] = "> LSBank - Reset System";




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
            //TODO: Funció del serial que printi el missatge de bevinguda(No se si sha de posar nomes un cop en tot el programa o cada cop que es reseteja)
            Serial_PrintaMissatge(const char missatge[])
            LEDS_EncenLed(LED_STATE_OK);
            LEDS_ApagaLed(LED_STATE_ALARM);
            TI_ResetTics(timerController);
            //Per tenir disponible un altre cop el protocol d'alarma
            Alarma = 1;
            Estat = 1;
            
            break;
        //Estat d'espera del hall
        case 1:
            //TODO: posar un if per si es detecta el HALL
                //dintre del if
                //TODO: Funcio del serial que printi el missatge: > LSBank - Open exterior door
                TI_ResetTics(timerController);
                Estat = 2;                
                break;
        //estat que espera dos segons un cop s'ha posat el hall
        case 2:
            if(TI_GetTics(timerController)>= 1000){
                        //TODO: Funcio del serial que printi el missatge: > LSBank - close/closed exterior door
                        SPE_PlayAcuteSound();
                        //TODO: Funcio del serial que printi el missatge: > LSBank - Enter PIN:
                        TI_ResetTics(timerController);
                        Estat = 3;
            }
            break; 
        case 3: 
                
            //comprovar si no han passat 2 minuts
            if(TI_GetTics(timerController)<= 60000){
                esCorrecte = 1;
                PIN_CORRECTE = 0;
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
                        //anira al protocol d'alarma
                        Estat = 4;
                    } else{

                        //posar > LSBank - Permision Denied
                        //posar > LSBank - Enter PIN
                    }
                    

                    
                } else {
                    //Si es correcte
                    INT_stop();
                    //TODO: Posar missatge de: > LSBank - Open Interior door
                    TI_ResetTics(timerController);
                    Estat = 5;

                }

            }
            else{
                //han passat els 2 minuts (ALARMA)
                Estat = 4;
            }
            break;
        //estat alarma
        case 4: 
            
            if(Alarma){
                SPE_PlayAlarmSound();
                //TODO: Serial posasr: > LSBank - Thief Intercepted 
                LEDS_EncenLed (LED_STATE_ALARM);
                LEDS_ApagaLed (LED_STATE_OK);
                TI_ResetTics(timerController);
                INT_stop();

                Alarma = 0;
            }
            
            if(TI_GetTics(timerController)<= 5000){
                //TODO: Posar missatge pel serial de :  > LSBank - Reset System: 

                //TODO: llegir la resposta amb el serial
                //TODO: Posar if comprovant si el Missatge es "Yes"
                    //Si es "Yes"
                    //TI_ResetTics(timerController);
                    //Anar al estat 0
                    
                //TODO: else 
                    //TODO: Posar missatge pel serial de :  > LSBank - Reset System:

            } else {
                TI_ResetTics(timerController);
                Estat = 0;
            }
            break;

        //Espera de 2 segons quan es correcte
        case 5: 
            if(TI_GetTics(timerController)>= 1000){
                //TODO: Printar > LSBank - Closed interior door
                SPE_PlayAcuteSound();
                Estat = 6; 
            }
            break;
        //Espera del botó per sortir
        case 6:
            if(CO_ExitRequested()){
                //TODO: printar: > LSBank - Exit Requested:

                //if "Yes"
                    //TODO: printar: > LSBank - Open bot doors
                    TI_ResetTics(timerController);
                    Estat = 7;
                //else ("No")
                    //Estat = 4;


            }

            break;
        //espera de 2 segons quan s'ha acceptat la sortida del bank
        case 7: 
            if(TI_GetTics(timerController)>= 1000){
                //TODO: printar > LSBank - Close both doors
                SPE_PlayAcuteSound();
                Estat = 0;
            }
    }
}

