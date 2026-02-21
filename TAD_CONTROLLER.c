/*
 * File:   TAD_CONTROLLER.c
 * Author: ACER
 *
 * Created on 13 de febrero de 2026, 17:17
 */


#include <xc.h>
#include "TAD_CONTROLLER.h"
#include "TAD_SERIAL.h"
#include "TAD_TIMER.h"
#include "TAD_HALL.h"
#include "TAD_LEDS.h"

static unsigned char PIN_CORRECTE_1[] = "1511MTV";
static unsigned char PIN_CORRECTE_2[] = "2806AGN";
static unsigned char SOLICITUD_ACCEPTADA[] = "Yes";
static unsigned char SOLICITUD_DENEGADA[] = "No";

const char MISSATGE_BENVINGUDA[] = "\r> LSBank - New Day!\r\n";
const char PORTA_EXTERIOR_OBERTA[] = "\r> LSBank - Open exterior door\r\n";
const char PORTA_EXTERIOR_TANCADA[] = "\r> LSBank - Close exterior door\r\n";

const char MISSATGE_ENTRA_PIN[] = "\r> LSBank - Enter PIN: ";

const char PERMIS_DENEGAT[] = "\r> LSBank - Permission Denied\r\n";

const char PORTA_INTERIOR_OBERTA[] = "\r> LSBank - open interior door\r\n";
const char PORTA_INTERIOR_TANCADA[] = "\r> LSBank - close interior door\r\n";

const char EXIT_REQUESTED[] = "\r> LSBank - Exit Requested\r";

const char DUES_PORTES_OBERTES[] = "\r> LSBank - Open both doors\r\n";
const char DUES_PORTES_TANCADES[] = "\r> LSBank - Close both doors\r\n";

const char LLADRE_INTERCEPTAT[] = "\r> LSBank - Thief Intercepted\r\n";
const char RESET_SISTEMA[] = "\r> LSBank - Reset System\r\n";

char i = 0;

static unsigned char timerPortaExterior;

void Init_Controller(void){
    TI_NewTimer(&timerPortaExterior);
}

void motorController(void) {
    static char estat = 0;
    
    switch (estat) {
        case 0:
            LEDS_EncenLed(LED_STATE_OK);
            LEDS_ApagaLed(LED_STATE_ALARM);
            Serial_PrintaMissatge(MISSATGE_BENVINGUDA);
            
            /*if (MISSATGE_BENVINGUDA[i] != '\0') {
                if (TXIF == 1) {                      
                    Serial_PutChar(MISSATGE_BENVINGUDA[i]);
                    i++;
                }
            }
            else {
                i = 0;
                estat = 1;
            }*/
            estat = 1;
            
            break;
            
        case 1:
            if(Hall_Detectat()) {
                i = 0;
                estat = 2;
            }
     
            break;
            
        case 2:
            if(PORTA_EXTERIOR_OBERTA[i] != '\0') {
                if(TXIF) {
                    Serial_PutChar(PORTA_EXTERIOR_OBERTA[i]);
                    i++;
                }
            } else {
                i = 0;
                
                // Activar speaker
                LATDbits.LATD3 = 1;   // ejemplo buzzer

                TI_ResetTics(timerPortaExterior);
                
                estat = 3;
            }
            
            break;
            
        case 3:
            if(TI_GetTics(timerPortaExterior) >= 1000) {
                LATDbits.LATD3 = 0;   // apagar buzzer

                estat = 4;
            }
            
            break;
            
        case 4:
            if(PORTA_EXTERIOR_TANCADA[i] != '\0') {
                if(TXIF) {
                    Serial_PutChar(PORTA_EXTERIOR_TANCADA[i]);
                    i++;
                }
            }
            else {
                i = 0;
                estat = 5;
            }
            
            break;
            
        case 5:
//            if(!Hall_Detectat()) {
//                estat = 1;
//            }
            if(MISSATGE_ENTRA_PIN[i] != '\0') {
                if(TXIF) {
                    Serial_PutChar(MISSATGE_ENTRA_PIN[i]);
                    i++;
                }
            }
            else {
                i = 0;
                estat = 6;
            }
            
            break;
    }
}
