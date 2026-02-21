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
#include "TAD_SPEAKER.h"

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

char intents = 0;
char caractersPIN = 0;
unsigned char PIN[7];
static unsigned char timerController;

void Init_Controller(void){
    TI_NewTimer(&timerController);
}
void CO_NewKeyPressed(unsigned char key){
    if(caractersPIN<7){
        PIN[caractersPIN++] = key;
        Serial_PutChar(key);
    }
    
    
}
void motorController(void) {
    static char estat = 0;
    
    switch (estat) {
        case 0:
            LEDS_EncenLed(LED_STATE_OK);
            LEDS_ApagaLed(LED_STATE_ALARM);
            Serial_PrintaMissatge(MISSATGE_BENVINGUDA);
            
            
            estat = 1;
            
            break;
            
        case 1:
            if(Hall_Detectat()) {
                Serial_PrintaMissatge(PORTA_EXTERIOR_OBERTA);
                TI_ResetTics(timerController);
                estat = 2; 
            }
     
            break;
            
        case 2:
            if(TI_GetTics(timerController) >= 1000) {
                Serial_PrintaMissatge(PORTA_EXTERIOR_TANCADA);
                SPE_PlayAcuteSound();
                Serial_PrintaMissatge(MISSATGE_ENTRA_PIN);
                TI_ResetTics(timerController);
                estat = 3;

                
            }
            
            break;
        case 3: 
            if(TI_GetTics(timerController) >= 100){
                SPE_PlayPressureSound();
                estat = 4;
            }
            
            break;
            
 
        case 4:
               if(TI_GetTics(timerController) >= 60000){
                   if(caractersPIN <8){
                       
                   }
                   
            }
    }
}
