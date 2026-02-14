/*
 * File:   TAD_KEYPAD.c
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:34
 */


#include "TAD_TECLAT.h"

static unsigned char timerRebots;
static unsigned char timerSMS;

static unsigned char estatTeclat;
static unsigned char filaActiva;

static char lastChar;
static unsigned char newChar;

static char lastKey;
static unsigned char pulsacions;

static const char SMS_0[FILES][COLUMNES] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'0','0','0'}     
};

static const char SMS_1[FILES][COLUMNES] = {
    {'1','A','D'},
    {'G','J','M'},
    {'P','T','W'},
    {'0','0','0'}
};

static const char SMS_2[FILES][COLUMNES] = {
    {'1','B','E'},
    {'H','K','N'},
    {'R','U','X'},
    {'0','0','0'}
};

static const char SMS_3[FILES][COLUMNES] = {
    {'1','C','F'},
    {'I','L','O'},
    {'S','V','Y'},
    {'0','0','0'}
};

#define KEY_PRESSED   (!PORTBbits.RB1 || !PORTBbits.RB2 || !PORTBbits.RB3)

static char DetectKey(unsigned char fila) {
    if (!PORTBbits.RB1) return SMS_0[fila][0];
    if (!PORTBbits.RB2) return SMS_0[fila][1];
    if (!PORTBbits.RB3) return SMS_0[fila][2];
    return 0;
}

static char SMS_Convert(unsigned char fila, unsigned char columna, unsigned char prem) {
    if (prem == 1) return SMS_1[fila][columna];
    if (prem == 2) return SMS_2[fila][columna];
    if (prem == 3) return SMS_3[fila][columna];
    return SMS_1[fila][columna];
}

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
    ADCON1 = 0x0F;

    TI_NewTimer(&timerRebots);
    TI_NewTimer(&timerSMS);

    estatTeclat = 0;
    filaActiva  = 0;

    lastChar = 0;
    newChar  = 0;

    lastKey = 0;
    pulsacions = 0;
}

void MotorTeclat(void) {

    char key;
    unsigned char col = 0;

    switch (estatTeclat) {
        case 0: case 1: case 2: case 3:
            LATAbits.LATA0 = (estatTeclat != 0);
            LATAbits.LATA1 = (estatTeclat != 1);
            LATAbits.LATA2 = (estatTeclat != 2);
            LATAbits.LATA3 = (estatTeclat != 3);

            filaActiva = estatTeclat;

            if (KEY_PRESSED) {
                TI_ResetTics(timerRebots);
                estatTeclat = 4;
            } else {
                estatTeclat = (estatTeclat + 1) & 0x03;
            }
        break;

        /* ---------- Rebots ---------- */
        case 4:
            if (TI_GetTics(timerRebots) >= REBOT_TICS) {
                if (KEY_PRESSED) estatTeclat = 5;
                else estatTeclat = 0;
            }
        break;

        /* ---------- Lectura Tecla ---------- */
        case 5:
            if (!PORTBbits.RB1) col = 0;
            else if (!PORTBbits.RB2) col = 1;
            else if (!PORTBbits.RB3) col = 2;

            key = SMS_0[filaActiva][col];

            if (key == lastKey) {
                pulsacions++;
                if (pulsacions > 3) pulsacions = 1;
            } else {
                pulsacions = 1;
                lastKey = key;
            }

            lastChar = SMS_Convert(filaActiva, col, pulsacions);
            newChar  = 0;

            TI_ResetTics(timerSMS);
            estatTeclat = 6;
        break;

        /* ---------- Espera ---------- */
        case 6:
            if (!KEY_PRESSED) {
                TI_ResetTics(timerRebots);
                estatTeclat = 7;
            }
        break;

        /* ---------- Confirmaci� ---------- */
        case 7:
            if (TI_GetTics(timerRebots) >= REBOT_TICS) {
                if (!KEY_PRESSED) {
                    newChar = 1;
                    estatTeclat = 8;
                } else {
                    estatTeclat = 6;
                }
            }
        break;

        /* ---------- Timeout SMS ---------- */
        case 8:
            if (TI_GetTics(timerSMS) >= SMS_TIMEOUT_TICS) {
                lastKey = 0;
                pulsacions = 0;
                estatTeclat = 0;
            } else if (KEY_PRESSED) {
                estatTeclat = filaActiva;
            }
        break;
    }
}

char Teclat_GetLastChar(void) {
    newChar = 0;
    return lastChar;
}

unsigned char Teclat_HasNewChar(void) {
    return newChar;
}

void Teclat_Reset(void) {
    lastChar = 0;
    newChar = 0;
    lastKey = 0;
    pulsacions = 0;
}