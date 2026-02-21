#ifndef XC_H
#define XC_H

// Mock of XC8 registers/types for host-side testing with GCC.

// Macro to ignore __interrupt() syntax
#define __interrupt()

// TRISD
typedef struct {
    unsigned char TRISD0;
    unsigned char TRISD1;
    unsigned char TRISD2;
    unsigned char TRISD3;
} TRISDBITS_t;

// TRISA
typedef struct {
    unsigned char RA0;
    unsigned char RA1;
    unsigned char RA2;
    unsigned char RA3;
    unsigned char RA4;
} TRISABITS_t;

// TRISB
typedef struct {
    unsigned char RB0;
    unsigned char RB1;
    unsigned char RB2;
    unsigned char RB3;
    unsigned char RB4;
    unsigned char RB5;
    unsigned char RB6;
    unsigned char RB7;
} TRISBBITS_t;

// TRISC
typedef struct {
    unsigned char TRISC0;
    unsigned char TRISC1;
    unsigned char TRISC2;
    unsigned char TRISC3;
    unsigned char TRISC4;
    unsigned char TRISC5;
    unsigned char TRISC6;
    unsigned char TRISC7;
} TRISCBITS_t;

// PORTC
typedef struct {
    unsigned char RC0;
    unsigned char RC1;
    unsigned char RC2;
    unsigned char RC3;
    unsigned char RC4;
    unsigned char RC5;
    unsigned char RC6;
    unsigned char RC7;
} PORTCBITS_t;

// LATD
typedef struct {
    unsigned char LATD0;
    unsigned char LATD1;
    unsigned char LATD2;
    unsigned char LATD3;
} LATDBITS_t;

// LATA
typedef struct {
    unsigned char LATA0;
    unsigned char LATA1;
    unsigned char LATA2;
    unsigned char LATA3;
    unsigned char LATA4;
} LATABITS_t;

// PORTB
typedef struct {
    unsigned char RB0;
    unsigned char RB1;
    unsigned char RB2;
    unsigned char RB3;
    unsigned char RB4;
    unsigned char RB5;
    unsigned char RB6;
    unsigned char RB7;
} PORTBBITS_t;

// PORTA
typedef struct {
    unsigned char RA0;
    unsigned char RA1;
    unsigned char RA2;
    unsigned char RA3;
    unsigned char RA4;
} PORTABITS_t;

// INTCON
typedef struct {
    unsigned char TMR0IF;
    unsigned char TMR0IE;
    unsigned char GIE;
    unsigned char PEIE;
} INTCONBITS_t;

// INTCON2
typedef struct {
    unsigned char RBPU;
} INTCON2BITS_t;

// TXSTA
typedef struct {
    unsigned char SYNC;
    unsigned char BRGH;
    unsigned char TXEN;
    unsigned char TRMT;
} TXSTABITS_t;

// RCSTA
typedef struct {
    unsigned char SPEN;
    unsigned char CREN;
} RCSTABITS_t;

// BAUDCON
typedef struct {
    unsigned char BRG16;
} BAUDCONBITS_t;

// PIR1
typedef struct {
    unsigned char RCIF;
    unsigned char TXIF;
    unsigned char TMR1IF;
    unsigned char CCP1IF;
} PIR1BITS_t;

// PIE1
typedef struct {
    unsigned char TMR1IE;
    unsigned char CCP1IE;
} PIE1BITS_t;

// T1CON
typedef struct {
    unsigned char TMR1ON;
    unsigned char T1CKPS0;
    unsigned char T1CKPS1;
} T1CONBITS_t;

// CCP1CON
typedef struct {
    unsigned char CCP1M0;
    unsigned char CCP1M1;
    unsigned char CCP1M2;
    unsigned char CCP1M3;
} CCP1CONBITS_t;

extern TRISDBITS_t TRISDbits;
extern TRISABITS_t TRISAbits;
extern TRISBBITS_t TRISBbits;
extern TRISCBITS_t TRISCbits;
extern LATDBITS_t LATDbits;
extern LATABITS_t LATAbits;
extern PORTBBITS_t PORTBbits;
extern PORTABITS_t PORTAbits;
extern PORTCBITS_t PORTCbits;
extern INTCONBITS_t INTCONbits;
extern INTCON2BITS_t INTCON2bits;
extern TXSTABITS_t TXSTAbits;
extern RCSTABITS_t RCSTAbits;
extern BAUDCONBITS_t BAUDCONbits;
extern PIR1BITS_t PIR1bits;
extern PIE1BITS_t PIE1bits;
extern T1CONBITS_t T1CONbits;
extern CCP1CONBITS_t CCP1CONbits;

extern unsigned int TMR0;
extern unsigned char T0CON;
extern unsigned char TMR0IF;
extern unsigned char SPBRG;
extern unsigned char RCREG;
extern unsigned char TXREG;
extern unsigned int TMR1;
extern unsigned int CCPR1;
extern unsigned char ADCON1;

static inline void di(void) { }
static inline void ei(void) { }

#endif
