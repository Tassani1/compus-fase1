/*
 * File:   TAD_KEYPAD.c
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:34
 */

#include "TAD_TECLAT.h"
#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_CONTROLLER.h"

static unsigned char timerRebots;
static unsigned char timerSMS;

static char auxChar;
static char teclaPremuda = 0;

static unsigned char smsIndex;
static char teclaBaseActual;

static const char SMS_0[FILES][COLUMNES] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}     
};

static const char SMS_1[FILES][COLUMNES] = {
    {'1','A','D'},
    {'G','J','M'},
    {'P','T','W'},
    {'*','0','#'}
};

static const char SMS_2[FILES][COLUMNES] = {
    {'1','B','E'},
    {'H','K','N'},
    {'R','U','X'},
    {'*','0','#'}
};

static const char SMS_3[FILES][COLUMNES] = {
    {'1','C','F'},
    {'I','L','O'},
    {'S','V','Y'},
    {'*','0','#'}
};

void teclat_init(void) {

    /* Files: Sortides */
    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;

    /* Columnes: Entrades */
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;
    TRISBbits.RB3 = 1;

    INTCON2bits.RBPU = 0;
    ADCON1 = 0x0F;

    timer_newTimer(&timerRebots);
    timer_newTimer(&timerSMS);

    auxChar = 0;
    smsIndex = 0;
    teclaBaseActual = 0;
    
    //LED
    TRISAbits.RA4 = 0;
}

void teclat_motor(void) {
    static unsigned char estatTeclat = 0;
    static char fila;
    static unsigned char columna = 0;
    char teclaBase;
    
    switch(estatTeclat) {
        //========================
        // FILA 0
        //========================
        case 0:
            LATAbits.LATA0 = 1;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
            fila = 0;
            estatTeclat = 1;     // esperar 1 ciclo
        break;

        case 1:   // lectura fila 0
            if(PREMUT()){
                estatTeclat = 8;  // ir a rebote
            } else {
                estatTeclat = 2;
            }
        break;

        //========================
        // FILA 1
        //========================
        case 2:
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 1;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
            fila = 1;
            estatTeclat = 3;
        break;

        case 3:   // lectura fila 1
            if(PREMUT()){
                estatTeclat = 8;
            } else {
                estatTeclat = 4;
            }
        break;

        //========================
        // FILA 2
        //========================
        case 4:
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 0;
            fila = 2;
            estatTeclat = 5;
        break;

        case 5:   // lectura fila 2
            if(PREMUT()){
                estatTeclat = 8;
            } else {
                estatTeclat = 6;
            }
        break;

        //========================
        // FILA 3
        //========================
        case 6:
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 1;
            fila = 3;
            estatTeclat = 7;
        break;

        case 7:   // lectura fila 3
            if(PREMUT()){
                estatTeclat = 8;
            } else {
                estatTeclat = 0;   // volver a empezar
            }
        break;

        //========================
        // REBOTE
        //========================
        case 8:
            timer_resetTics(timerRebots);
            estatTeclat = 9;
        break;

        case 9:
            if(timer_getTics(timerRebots) >= REBOT_TICS){
                if(PREMUT()){
                    estatTeclat = 10;   // tecla v�lida
                } else {
                    estatTeclat = 0;    // falso disparo
                }
            }
        break;

        //========================
        // TECLA CONFIRMADA
        //========================
        case 10:
            // Aqu� lees columna
            if(PORTBbits.RB1) columna = 0;
            else if(PORTBbits.RB2) columna = 1;
            else if(PORTBbits.RB3) columna = 2;
            else{
                // No hi ha columna vàlida: tornem a escanejar.
                estatTeclat = 0;
                break;
            }

            // Aqu� ya haces tu l�gica SMS
            teclaBase = getSMS_0(fila,columna);
            teclaPremuda = 1;

            // Si canvia de tecla, confirmem immediatament la pendent
            // per evitar esperar sempre el timeout SMS.
            if((auxChar != 0) && (teclaBase != teclaBaseActual)){
                controller_newKeyPressed(auxChar);
                auxChar = 0;
                smsIndex = 0;
            }

            if(teclaBase == teclaBaseActual){
                smsIndex++;
                if(smsIndex >= 4){
                    smsIndex = 0;
                }
            }
            else{
                teclaBaseActual = teclaBase;
                smsIndex = 0;
            }

            if (smsIndex == 0){
                auxChar = SMS_0[fila][columna];
            } else if (smsIndex == 1){
                auxChar = SMS_1[fila][columna];
            } else if (smsIndex == 2){
                auxChar = SMS_2[fila][columna];
            } else {
                auxChar = SMS_3[fila][columna];
            }

            timer_resetTics(timerSMS);
            estatTeclat = 11;
        break;

        //========================
        // ESPERAR SOLTAR TECLA
        //========================
        case 11:
            if(!PREMUT()){
                timer_resetTics(timerRebots);
                estatTeclat = 12;
            }
        break;

        case 12:
            if(timer_getTics(timerRebots) >= REBOT_TICS){
                if(!PREMUT()){
                    estatTeclat = 0;
                } else {
                    estatTeclat = 11;
                }
            }
        break;
    }
    
    if(auxChar != 0) {
        if(timer_getTics(timerSMS) >= SMS_TIMEOUT_TICS) {
            //serial_putChar(auxChar);
            controller_newKeyPressed(auxChar);
            auxChar = 0;
            smsIndex = 0;
            teclaBaseActual = 0;
        }
    }
}

char getSMS_0(char fila, char columna) {
    return SMS_0[fila][columna];
}

void teclat_reset(void) {
    auxChar = 0;
    smsIndex = 0;
    teclaBaseActual = 0;
}
