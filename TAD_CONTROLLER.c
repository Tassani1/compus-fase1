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

static char PIN_CORRECTE[] = "12";
static unsigned char PIN_CORRECTE_1[] = "1511MTV\0";
static unsigned char PIN_CORRECTE_2[] = "2806AGN";
static char SOLICITUD_ACCEPTADA[] = "Yes";
static char SOLICITUD_DENEGADA[] = "No";
static char missatgeRebut[20];
unsigned char iMissatgeRebut = 0;

const char MISSATGE_BENVINGUDA[] = "\n\r> LSBank - New Day!\r";
const char PORTA_EXTERIOR_OBERTA[] = "\n\r> LSBank - Open exterior door\r\n";
const char PORTA_EXTERIOR_TANCADA[] = "\r> LSBank - Close exterior door\r\n";

const char MISSATGE_ENTRA_PIN[] = "\r> LSBank - Enter PIN: ";

const char PERMIS_DENEGAT[] = "\n\r> LSBank - Permission Denied\r\n";

const char PORTA_INTERIOR_OBERTA[] = "\n\r> LSBank - Open interior door\r\n";
const char PORTA_INTERIOR_TANCADA[] = "\r> LSBank - Close interior door\r\n";

const char EXIT_REQUESTED[] = "\r> LSBank - Exit Requested: ";

const char DUES_PORTES_OBERTES[] = "\n\r> LSBank - Open both doors\r\n";
const char DUES_PORTES_TANCADES[] = "\r> LSBank - Close both doors\r";

const char LLADRE_INTERCEPTAT[] = "\n\r> LSBank - Thief Intercepted\r";
const char RESET_SISTEMA[] = "\n\r> LSBank - Reset System: ";
//const char RESET_SISTEMA_CICLIC[] = "\r> LSBank - Reset System: ";

char intents = 0;
char caractersPIN = 0;
char PIN[8];
static unsigned char timerController;
char exitRequestedPremut = 0;

void controller_init(void){
    timer_newTimer(&timerController);
}
void controller_newKeyPressed(unsigned char key){
    if(caractersPIN<7){
        PIN[caractersPIN++] = key;
        PIN[caractersPIN] = '\0'; 
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
    if(iMissatgeRebut < (unsigned char)(sizeof(missatgeRebut) - 1)){
        missatgeRebut[iMissatgeRebut++] = lletra;
        missatgeRebut[iMissatgeRebut] = '\0';
    } else{
        // Evitem overflow mantenint sempre una cadena acabada en '\0'.
        missatgeRebut[sizeof(missatgeRebut) - 1] = '\0';
    }
}

void engegarAlarma(){
    //anar estat alarma
    leds_apagaLed(LED_STATE_OK);
    leds_encenLed(LED_STATE_ALARM);
    intensity_stop();
    timer_resetTics(timerController);
    serial_printaMissatge(LLADRE_INTERCEPTAT); 
    speaker_playAlarmSound();
    serial_esperaYesONo();
}

void controller_motor(void) {
    static char estat = 0;
    
    switch (estat) {
        case 0:
            leds_encenLed(LED_STATE_OK);
            leds_apagaLed(LED_STATE_ALARM);
            
            serial_printaMissatge(MISSATGE_BENVINGUDA);
           
            intents = 0;
            caractersPIN = 0;
            iMissatgeRebut = 0;
            missatgeRebut[0] = '\0';
            exitRequestedPremut = 0;
            
            speaker_stopSound();
            
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
                if(caractersPIN >=7){
                    
                    if(string_equals(PIN, PIN_CORRECTE_1)){
                        estat = 5;
                        intents = 0;
                    } else {
                        if(++intents >2){
                            engegarAlarma();
                            serial_printaMissatge(RESET_SISTEMA);
                            estat = 10;

                        }else{
                            //iMissatgeRebut = 0;
                            caractersPIN = 0;
                            serial_printaMissatge(PERMIS_DENEGAT);
                            serial_printaMissatge(MISSATGE_ENTRA_PIN);

                        }
                    }
                }    
            } else{
                engegarAlarma();
                serial_printaMissatge(RESET_SISTEMA);
                estat = 10;                
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
            if(exitRequestedPremut){
                
                exitRequestedPremut = 0;
                serial_printaMissatge(EXIT_REQUESTED);
                
                iMissatgeRebut = 0;
                serial_esperaYesONo();
                
                estat = 8;
            }
            break;
        case 8: 
            if(string_equals(SOLICITUD_ACCEPTADA, missatgeRebut)){
                iMissatgeRebut = 0;
                serial_printaMissatge(DUES_PORTES_OBERTES);
                timer_resetTics(timerController);
                estat = 9;
            } else{
                if(string_equals(SOLICITUD_DENEGADA, missatgeRebut)){
                    timer_resetTics(timerController);
                    engegarAlarma();
                    estat = 10;
                    iMissatgeRebut = 0;
                    missatgeRebut[0] = '\0';
                }
            }
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
            if(serial_respostaDisponible()){
                if(string_equals(SOLICITUD_ACCEPTADA, missatgeRebut)){
                    iMissatgeRebut = 0;
                    missatgeRebut[0] = '\0';
                    estat = 0;          // sortir immediat
                    
                    break;
                }
                iMissatgeRebut = 0;    // resposta incorrecta
                missatgeRebut[0] = '\0';
                serial_printaMissatge(RESET_SISTEMA);
                serial_esperaYesONo();
            }

            /*if(timer_getTics(timerController) >= 60000){
                estat = 0;             // timeout
            }*/
            break;       
    }
}
