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
static char hallDetected = 0;
static char numeLletres = 0;

static unsigned char PIN_CORRECTE[] = "1511MTV";
static unsigned char SOLICITUD_ACCEPTADA[] = "Yes";
static unsigned char SOLICITUD_DENEGADA[] = "No";

const char MISSATGE_BENVINGUDA[] = "\r> LSBank - New Day! \r\n\0";
const char PORTA_EXTERIOR_OBERTA[] = "\r> LSBank - open exterior door\r\n";
const char PORTA_EXTERIOR_TANCADA[] = "\r> LSBank - close exterior door\r\n";

const char MISSATGE_ENTRA_PIN[] = "\r> LSBank - Enter PIN\r";

const char PERMIS_DENEGAT[] = "\r> LSBank - Permission Denied\r\n";

const char PORTA_INTERIOR_OBERTA[] = "\r> LSBank - open interior door\r\n";
const char PORTA_INTERIOR_TANCADA[] = "\r> LSBank - close interior door\r\n";

const char EXIT_REQUESTED[] = "\r> LSBank - Exit Requested\r";

const char DUES_PORTES_OBERTES[] = "\r> LSBank - Open both doors\r\n";
const char DUES_PORTES_TANCADES[] = "\r> LSBank - Close both doors\r\n";

const char LLADRE_INTERCEPTAT[] = "\r> LSBank - Thief Intercepted\r\n";
const char RESET_SISTEMA[] = "\r> LSBank - Reset System\r\n";




void Init_Controller(void){
    TI_NewTimer(&timerController);
}

void CO_OpenExteriorDoor(void) {
    hallDetected = 1;
}



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
            //Funció del serial que printi el missatge de bevinguda(No se si sha de posar nomes un cop en tot el programa o cada cop que es reseteja)
            Serial_PrintaMissatge(MISSATGE_BENVINGUDA);
            LEDS_EncenLed(LED_STATE_OK);
            LEDS_ApagaLed(LED_STATE_ALARM);
            TI_ResetTics(timerController);
            //Per tenir disponible un altre cop el protocol d'alarma
            Alarma = 1;
            Estat = 1;
            
            break;
        //Estat d'espera del hall
        case 1:
            //posar un if per si es detecta el HALL
                //dintre del if
                // Funcio del serial que printi el missatge: > LSBank - Open exterior door
                Serial_PrintaMissatge(PORTA_EXTERIOR_OBERTA);
                TI_ResetTics(timerController);
                Estat = 2;                
                break;
        //estat que espera dos segons un cop s'ha posat el hall
        case 2:
            if(TI_GetTics(timerController)>= 1000){
                        // Funcio del serial que printi el missatge: > LSBank - close/closed exterior door
                        Serial_PrintaMissatge(PORTA_EXTERIOR_TANCADA);
                        SPE_PlayAcuteSound();
                        // Funcio del serial que printi el missatge: > LSBank - Enter PIN:
                        Serial_PrintaMissatge(MISSATGE_ENTRA_PIN);
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
                //TODO: Posar un if per quan t'introdueixin una tecla i que la vagi guardant.
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
                        Serial_PrintaMissatge(PERMIS_DENEGAT);
                        //posar > LSBank - Enter PIN
                        Serial_PrintaMissatge(MISSATGE_ENTRA_PIN);
                    }
                    

                    
                } else {
                    //Si es correcte
                    INT_stop();
                    // Posar missatge de: > LSBank - Open Interior door
                    Serial_PrintaMissatge(PORTA_INTERIOR_OBERTA);
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
                // Serial posasr: > LSBank - Thief Intercepted
                Serial_PrintaMissatge(LLADRE_INTERCEPTAT);
                LEDS_EncenLed (LED_STATE_ALARM);
                LEDS_ApagaLed (LED_STATE_OK);
                TI_ResetTics(timerController);
                INT_stop();

                Alarma = 0;
            }
            
            if(TI_GetTics(timerController)<= 5000){
                //Posar missatge pel serial de :  > LSBank - Reset System: 
                Serial_PrintaMissatge(RESET_SISTEMA);
                //TODO: llegir la resposta amb el serial

                //TODO: Posar if comprovant si el Missatge es "Yes"
                /*if(Serial_CmdReady()){
                    char *cmd = Serial_GetCmd();

                    if(cmd[0]=='Y' && cmd[1]=='e' && cmd[2]=='s' && cmd[3]=='\0'){

                    // ACCEPTAT
                 }*/


                    //Si es "Yes"
                    //TI_ResetTics(timerController);
                    //Anar al estat 0
                    
                //TODO: else 
                    //TODO: Posar missatge pel serial de :  > LSBank - Reset System:
                    Serial_PrintaMissatge(RESET_SISTEMA);

            } else {
                TI_ResetTics(timerController);
                Estat = 0;
            }
            break;

        //Espera de 2 segons quan es correcte
        case 5: 
            if(TI_GetTics(timerController)>= 1000){
                // Printar > LSBank - Closed interior door
                Serial_PrintaMissatge(PORTA_INTERIOR_TANCADA);
                SPE_PlayAcuteSound();
                Estat = 6; 
            }
            break;
        //Espera del botó per sortir
        case 6:
            if(CO_ExitRequested()){
                // printar: > LSBank - Exit Requested:
                Serial_PrintaMissatge(EXIT_REQUESTED);
                Estat = 7;


            }

            break;
        case 7:
            if(numeros <= 3)
                // printar: > LSBank - Open both doors
                    Serial_PrintaMissatge(DUES_PORTES_OBERTES);
                    TI_ResetTics(timerController);
                    Estat = 8;
                //else ("No")
                    //Estat = 4;

            break;
        //espera de 2 segons quan s'ha acceptat la sortida del bank
        case 8: 
            if(TI_GetTics(timerController)>= 1000){
                // printar > LSBank - Close both doors
                Serial_PrintaMissatge(DUES_PORTES_TANCADES);
                SPE_PlayAcuteSound();
                Estat = 0;
            }
    }
}

