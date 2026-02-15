#include "xc.h"

TRISDBITS_t TRISDBits = {0};
LATDBITS_t LATDbits = {0};
INTCONBITS_t INTCONbits = {0};

unsigned int TMR0 = 0;
unsigned char T0CON = 0;
unsigned char TMR0IF = 0;
