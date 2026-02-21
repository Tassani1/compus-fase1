/*
 * File:   TAD_CONTROLLER.c
 * Author: Ari i Marc
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

void controller_init(void){
    timer_newTimer(&timerController);
}
void controller_newKeyPressed(unsigned char key){
    if(caractersPIN<7){
        PIN[caractersPIN++] = key;
        serial_putChar(key);
    }
    
    
}
void controller_motor(void) {
    static char estat = 0;
    
    switch (estat) {
        case 0:
            leds_encenLed(LED_STATE_OK);
            leds_apagaLed(LED_STATE_ALARM);
            serial_printaMissatge(MISSATGE_BENVINGUDA);
            
            
            estat = 1;
            
            break;
            
        case 1:
            if(hall_detectat()) {
                serial_printaMissatge(PORTA_EXTERIOR_OBERTA);
                timer_resetTics(timerController);
                estat = 2; 
            }
     
            break;
            
        case 2:
            if(timer_getTics(timerController) >= 1000) {
                serial_printaMissatge(PORTA_EXTERIOR_TANCADA);
                speaker_playAcuteSound();
                serial_printaMissatge(MISSATGE_ENTRA_PIN);
                timer_resetTics(timerController);
                estat = 3;

                
            }
            
            break;
        case 3: 
            if(timer_getTics(timerController) >= 100){
                speaker_playPressureSound();
                estat = 4;
            }
            
            break;
            
 
        case 4:
               if(timer_getTics(timerController) >= 60000){
                   if(caractersPIN <8){
                       
                   }
                   
            }
    }
}
