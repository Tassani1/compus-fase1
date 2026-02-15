/*
 * File:   TAD_KEYPAD.c
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:34
 */


#include "TAD_TECLAT.h"
#include "TAD_SERIAL.h"

static unsigned char timerRebots;
static unsigned char timerSMS;

static char lastChar = '0';
static unsigned char newChar;
static char auxChar;
static char teclaPremuda = 0;

static unsigned char pulsacions;

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

void Init_Teclat(void) {

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
    ADCON1 = 0xFF;

    TI_NewTimer(&timerRebots);
    TI_NewTimer(&timerSMS);

    lastChar = 0;
    newChar  = 0;
    
    pulsacions = 0;
}

void MotorTeclat(void) {
    static unsigned char estatTeclat = 0;
    char fila;
    unsigned char columna = 0;

    switch (estatTeclat) {
        case 0:
            if(!PREMUT()) {
                LATAbits.LATA0=1;
                LATAbits.LATA1=0;
                LATAbits.LATA2=0;
                LATAbits.LATA3=0;
                estatTeclat = 1;
                fila = 0;
            }
            else {
                estatTeclat = 4;
            }
            break;
            
        case 1:
            if(!PREMUT()) {
                LATAbits.LATA0=0;
                LATAbits.LATA1=1;
                LATAbits.LATA2=0;
                LATAbits.LATA3=0;
                estatTeclat = 2;
                fila = 1;
            }
            else {
                estatTeclat = 4;
            }
            break;
            
        case 2:
            if(!PREMUT()) {
                LATAbits.LATA0=0;
                LATAbits.LATA1=0;
                LATAbits.LATA2=1;
                LATAbits.LATA3=0;
                estatTeclat = 3;
                fila = 2;
            }
            else {
                estatTeclat = 4;
            }
            break;
            
        case 3:
            if(!PREMUT()) {
                LATAbits.LATA0=0;
                LATAbits.LATA1=0;
                LATAbits.LATA2=0;
                LATAbits.LATA3=1;
                estatTeclat = 0;
                fila = 3;
            }
            else {
                estatTeclat = 4;
            }
            break;
            
        case 4:
            TI_ResetTics(timerRebots);
            estatTeclat = 5;
        break;
        
        case 5:
            if(TI_GetTics(timerRebots) >= REBOT_TICS) {
                if(PREMUT()) {
                    estatTeclat = 6;
                }
                else {
                    estatTeclat = 0;
                }
            }
        break;
        
        case 6:
            if(PORTBbits.RB1){  
                columna=0;  
            } else if(PORTBbits.RB2){
                columna=1;  
            } else if(PORTBbits.RB3){
                columna=2;  
            }
   
            lastChar = newChar;
            newChar = getSMS_0(fila,columna);
            auxChar = newChar;
            teclaPremuda = 1;
            
            if (newChar == lastChar){
                pulsacions++;
                if (pulsacions == 1){
                        auxChar = SMS_1[fila][columna];
                } else if (pulsacions == 2){
                    auxChar = SMS_2[fila][columna];
                } else if (pulsacions == 3){
                    auxChar = SMS_3[fila][columna];
                    lastChar = 0;
                } else if (pulsacions == 4){
                    pulsacions = 0;
                }
                if (TXSTAbits.TRMT) {
                    Serial_PutChar(auxChar);
                }
            }
            else{
                pulsacions = 0;
                if (TXSTAbits.TRMT) {
                    Serial_PutChar(auxChar);
                }
            }
            estatTeclat=7;
        break;
        
        case 7:
            if (!PREMUT()) {
                TI_ResetTics(timerRebots);
                estatTeclat=8;
            }
            break;
            
        case 8:
            if (TI_GetTics(timerRebots) >= REBOT_TICS){
                if(!PREMUT()) {
                    TI_ResetTics(timerRebots);
                    estatTeclat = 0;
                }
                else {
                    estatTeclat = 7;
                }
            }
            break;
    }
}

//char Teclat_GetLastChar(void) {
//    newChar = 0;
//    return lastChar;
//}
//
//unsigned char Teclat_HasNewChar(void) {
//    return newChar;
//}

char getSMS_0(char fila, char columna) {
    return SMS_0[fila][columna];
}

void Teclat_Reset(void) {
    lastChar = 0;
    newChar = 0;
    pulsacions = 0;
}
