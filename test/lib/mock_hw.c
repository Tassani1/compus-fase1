#include "xc.h"

TRISDBITS_t TRISDbits = {0};
TRISABITS_t TRISAbits = {0};
TRISBBITS_t TRISBbits = {0};
TRISCBITS_t TRISCbits = {0};
LATDBITS_t LATDbits = {0};
LATABITS_t LATAbits = {0};
PORTBBITS_t PORTBbits = {0};
PORTABITS_t PORTAbits = {0};
PORTCBITS_t PORTCbits = {0};
INTCONBITS_t INTCONbits = {0};
INTCON2BITS_t INTCON2bits = {0};
TXSTABITS_t TXSTAbits = {0};
RCSTABITS_t RCSTAbits = {0};
BAUDCONBITS_t BAUDCONbits = {0};
PIR1BITS_t PIR1bits = {0};
PIE1BITS_t PIE1bits = {0};
T1CONBITS_t T1CONbits = {0};
CCP1CONBITS_t CCP1CONbits = {0};

unsigned int TMR0 = 0;
unsigned char T0CON = 0;
unsigned char TMR0IF = 0;
unsigned char SPBRG = 0;
unsigned char RCREG = 0;
unsigned char TXREG = 0;
unsigned int TMR1 = 0;
unsigned int CCPR1 = 0;
unsigned char ADCON1 = 0;
