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

#define MAX_MESSAGES 8  // Mida de la cua de missatges

static unsigned char Estat = 0;
static unsigned char* messageQueue[MAX_MESSAGES];  // Cua circular de missatges
static unsigned char queueHead = 0;  // Índex de lectura
static unsigned char queueTail = 0;  // Índex d'escriptura
static unsigned char queueCount = 0; // Nombre de missatges a la cua
static unsigned char* message;
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
    // Afegir missatge a la cua si hi ha espai
    if (queueCount < MAX_MESSAGES) {
        messageQueue[queueTail] = missatge;
        queueTail = (queueTail + 1) % MAX_MESSAGES;
        queueCount++;
    }
    // Si la cua està plena, el missatge es descarta
}

void Motor_Serial(){

    switch(Estat){

        case 0:
            // Comprovar si hi ha missatges a la cua
            if (queueCount > 0){
                // Agafar el següent missatge de la cua
                message = messageQueue[queueHead];
                queueHead = (queueHead + 1) % MAX_MESSAGES;
                queueCount--;
                i = 0;
                Estat = 1;
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
                        //resetejar variables i tornar a buscar més missatges
                        Estat = 0;
                        i = 0;
                    }
                    
            }
            
            break;

        case 2: 

            
    }

}