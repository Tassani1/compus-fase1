/*
 * File:   TAD_SERIAL.c
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:34
 */


#include "TAD_SERIAL.h"

//const char MSG_INICIAL[23] = "\r> LSBank - New Day!\r\n\0";
//
//char i = 0;

void Init_Serial(void){
    TRISCbits.TRISC6 = 1;  
    TRISCbits.TRISC7 = 1; 
    
    // TXSTA = 0x24

    // Bits importants del BAUDRATE --> SYNC (TXSTA), BRG16 (BAUDCON) i BRGH (TXSTA)
    TXSTAbits.SYNC = 0;    
    TXSTAbits.BRGH = 1; 
    BAUDCONbits.BRG16 = 0;                          // FOSC/[16 (n + 1)] on n = 64 --> Valor de SPBRG
    TXSTAbits.TXEN = 1;

    SPBRG = 64;         
    
    // RCSTA = 0x10

    RCSTAbits.SPEN = 1;    
    RCSTAbits.CREN = 1;     
}

unsigned char Serial_GetChar(void){
    return RCREG;
}

void Serial_PutChar(char lletra){
    TXREG = lletra;
}

//void motorSerial(void) {
//    static char estat = 0;
//    //TRISAbits.TRISA3 = 0;
//    
//    switch (estat) {
//        case 0:
//            LATAbits.LATA3 = 0;
//            if (MSG_INICIAL[i] != '\0') {
//                if (TXSTAbits.TRMT) {
//                    Serial_PutChar(MSG_INICIAL[i]);
//                    i++;
//                }
//            }
//            else {
//                i = 0;
//                estat = 1;
//            }
//            
//            break;
//            
//        case 1:
//            estat = 1;
//            LATAbits.LATA3 = 1;
//            break;
//    }
//}