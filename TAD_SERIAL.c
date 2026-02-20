/*
 * File:   TAD_SIO.c
 * Author: ACER
 *
 * Created on 20 de febrero de 2026, 9:16
 */


#include <xc.h>
#include "TAD_SERIAL.h"
#include <pic18f4321.h>

#define CONFIGURACIO_TXSTA 0x24
#define CONFIGURACIO_RCSTA 0x90
#define DIVISOR_BAUDRATE 64

static unsigned char Estat = 0;
static unsigned char* message;
static unsigned char startWriting = 0;
static unsigned char i = 0;

void SIO_Init(void) {
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    BAUDCONbits.BRG16 = 0;
    TXSTA = CONFIGURACIO_TXSTA;
    RCSTA = CONFIGURACIO_RCSTA;
    SPBRG = DIVISOR_BAUDRATE;
}

unsigned char SIO_RXAvail() {
    return ((PIR1bits.RCIF == 1) ? CERT : FALS);
}

unsigned char SIO_GetChar() {
    return RCREG;
}

unsigned char SIO_TXAvail(void) {
    return ((PIR1bits.TXIF == 1) ? CERT : FALS);
}

void SIO_PutChar (unsigned char Valor) {
    TXREG = Valor;
}
void SIO_WriteMessage(char *missatge){
    message = missatge;
    startWriting = 1;
}

void Motor_Serial(){

    switch(Estat){

        case 0:
            if (startWriting){
                Estat = 1;
            } else {
                //si demana lo del yes o no caldrà fer lo de casa
                
            }
            break;


        case 1:
        //Mentre la SIO estigui activada enviar 
            if(SIO_TXAvail()){
                    if(message[i] != '\0' && message[i] != '\r' && message[i] != '\n'){
                        SIO_PutChar(message[i]);
                        i++;
                    }
                    else {
                        //resetejar variables
                        Estat = 0;
                        startWriting = 0;
                        i = 0;
                    }
                    
            }
            
            break;

        case 2: 

            
    }

}