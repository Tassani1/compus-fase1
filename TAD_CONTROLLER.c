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
#include "TAD_INTENSITY.h"

static unsigned char PIN_CORRECTE[] = "11";
static unsigned char PIN_CORRECTE_1[] = "1511MTV";
static unsigned char PIN_CORRECTE_2[] = "2806AGN";
static char SOLICITUD_ACCEPTADA[] = "Yes";
static char SOLICITUD_DENEGADA[] = "No";
static char missatgeRebut[20];
static char iMissatgeRebut = 0;

const char MISSATGE_BENVINGUDA[] = "\r> LSBank - New Day!\r\n";
const char PORTA_EXTERIOR_OBERTA[] = "\r> LSBank - Open exterior door\r\n";
const char PORTA_EXTERIOR_TANCADA[] = "\r> LSBank - Close exterior door\r\n";

const char MISSATGE_ENTRA_PIN[] = "\r> LSBank - Enter PIN: ";

const char PERMIS_DENEGAT[] = "\n\r> LSBank - Permission Denied\r\n";

const char PORTA_INTERIOR_OBERTA[] = "\n\r> LSBank - open interior door\r\n";
const char PORTA_INTERIOR_TANCADA[] = "\r> LSBank - close interior door\r\n";

const char EXIT_REQUESTED[] = "\r> LSBank - Exit Requested: \r\n";

const char DUES_PORTES_OBERTES[] = "\r> LSBank - Open both doors\r\n";
const char DUES_PORTES_TANCADES[] = "\r> LSBank - Close both doors\r\n";

const char LLADRE_INTERCEPTAT[] = "\r> LSBank - Thief Intercepted\r\n";
const char RESET_SISTEMA[] = "\r> LSBank - Reset System\r\n";

char intents = 0;
char caractersPIN = 0;
unsigned char PIN[7];
static unsigned char timerController;
char exitRequestedPremut = 0;

void controller_init(void){
    timer_newTimer(&timerController);
}
void controller_newKeyPressed(unsigned char key){
    if(caractersPIN<7){
        PIN[caractersPIN++] = key;
        serial_putChar(key);
    }
    

}
void controller_exitRequested(void){
    exitRequestedPremut = 1;
}
int string_equals(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}
void controller_repChar(char lletra){
    missatgeRebut[iMissatgeRebut++] = lletra;
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
                intensity_start();
                estat = 4;
            }
            
            break;
            
 
        case 4:
            if(timer_getTics(timerController) <= 60000){
                if(caractersPIN >=2){
                    
                    if(string_equals(PIN, PIN_CORRECTE)){
                        estat = 5;
                        intents = 0;
                    } else {
                        if(++intents >2){
                            //anar estat alarma
                            estat = 10;
                            intents = 0;
                        }
                    }
                }    
            }
            break;
            
            //PIN CORRECTE
         
        case 5:
            intensity_stop();
            serial_printaMissatge(PORTA_INTERIOR_OBERTA);
            timer_resetTics(timerController);
            estat = 6;
            break;
            
            
        case 6:
            if(timer_getTics(timerController) >= 1000){
                
                serial_printaMissatge(PORTA_INTERIOR_TANCADA);
                speaker_playAcuteSound();
                estat = 7;
            }
            break;
        case 7:
            //leds_encenLed(LED_STATE_ALARM);
            if(exitRequestedPremut){
                
                exitRequestedPremut = 0;
                serial_printaMissatge(EXIT_REQUESTED);
                leds_apagaLed(LED_STATE_OK);
                serial_esperaYesONo();
                estat = 8;
            }
            break;
        case 8: 
            if(string_equals(SOLICITUD_ACCEPTADA, missatgeRebut)){
                serial_printaMissatge(DUES_PORTES_OBERTES);
                timer_resetTics(timerController);
                estat = 9;
            } else{
                if(string_equals(SOLICITUD_DENEGADA, missatgeRebut)){
                    timer_resetTics(timerController);
                    estat = 10;
                }
            }
            iMissatgeRebut = 0;
            break;
        case 9:
            if(timer_getTics(timerController) >= 1000){
                serial_printaMissatge(DUES_PORTES_TANCADES);
                speaker_playAcuteSound();
                estat = 0;
            }
            break;
            //ALARMA
        case 10:
            break;
            
             
            
               
                    
                    
                
            
            
         
            
    }
}
