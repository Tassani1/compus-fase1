/*
 * File:   TAD_SERIAL.c
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:34
 */


#include "TAD_SERIAL.h"
#include "TAD_CONTROLLER.h"

//const char MSG_INICIAL[23] = "\r> LSBank - New Day!\r\n\0";
//
//char i = 0;

#define MAX_MESSAGES 8  // Mida de la cua de missatges

static unsigned char* messageQueue[MAX_MESSAGES];  // Cua circular de missatges
static unsigned char queueHead = 0;  // Índex de lectura
static unsigned char queueTail = 0;  // Índex d'escriptura
static unsigned char queueCount = 0; // Nombre de missatges a la cua
static unsigned char* message;
static unsigned char i = 0;
char YesONo = 0;


void serial_init(void){
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

unsigned char serial_getChar(void){
    return RCREG;
}

void serial_putChar(char lletra){
    TXREG = lletra;
}

unsigned char serial_RXAvail() {
    return ((PIR1bits.RCIF == 1) ? CERT : FALS);
}

unsigned char serial_TXAvail(void) {
    return ((PIR1bits.TXIF == 1) ? CERT : FALS);
}
void serial_printaMissatge(const char *missatge){
    // Afegir missatge a la cua si hi ha espai
    if (queueCount < MAX_MESSAGES) {
        messageQueue[queueTail] = missatge;
        queueTail = (queueTail + 1) % MAX_MESSAGES;
        queueCount++;
    }
    // Si la cua està plena, el missatge es descarta
}
void serial_esperaYesONo() {
        YesONo = 1;
}

void serial_motor(){
    char variable;
    static unsigned char Estat = 0;
    switch(Estat){

        case 0:
            // Comprovar si hi ha missatges a la cua
            if(YesONo){
                YesONo = 0;
                Estat = 2;
            } else{
                if (queueCount > 0){
                    // Agafar el següent missatge de la cua
                    message = messageQueue[queueHead];
                    queueHead = (queueHead + 1) % MAX_MESSAGES;
                    queueCount--;
                    i = 0;
                    Estat = 1;
                }
            }
            
            break;


        case 1:
        //Mentre la SIO estigui activada enviar 
            if(serial_TXAvail()){
                    if(message[i] != '\0'){
                        serial_putChar(message[i]);
                        i++;
                    }
                    else {
                        //resetejar variables
                        Estat = 0;
                        i = 0;
                    }
                    
            }
            break;
        case 2: 
            if (serial_RXAvail()) {
                variable = serial_getChar();
                controller_repChar(variable);
                if (serial_TXAvail()) {
                    serial_putChar(variable);
                }
            }
              
    }
    
    


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
//                    serial_putChar(MSG_INICIAL[i]);
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