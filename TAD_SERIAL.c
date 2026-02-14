/*
 * File:   TAD_SERIAL.c
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:34
 */


#include "TAD_SERIAL.h"

const char MSG_INICIAL[24] = "\r> LSBank - New Day! \r\n\0";

char i = 0;

void Init_Serial(void){
    TRISCbits.TRISC6 = 1;  
    TRISCbits.TRISC7 = 1;  

    TXSTAbits.SYNC = 0;    
    TXSTAbits.BRGH = 1;    
    BAUDCONbits.BRG16 = 0; 

    SPBRG = 64;            

    RCSTAbits.SPEN = 1;    
    RCSTAbits.CREN = 1;    
    TXSTAbits.TXEN = 1; 
}

unsigned char Serial_GetChar(void){
    return RCREG;
}

void Serial_PutChar(char lletra){
    TXREG = lletra;
}

void motorSerial(void) {
    static char estat = 0;
    //TRISAbits.TRISA3 = 0;
    
    switch (estat) {
        case 0:
            if (i < 24) {
                if (PIR1bits.TXIF) {
                    Serial_PutChar(MSG_INICIAL[i]);
                    i++;
                }
            }
            else {
                i = 0;
                estat = 1;
            }
            
            break;
            
        case 1:
            //LATAbits.LATA3 = 0;
            break;
    }
}