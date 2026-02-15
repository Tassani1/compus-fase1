#ifndef XC_H
#define XC_H

// Minimal mock of XC8 registers/types for host-side testing with GCC.

typedef struct {
    unsigned char TRISD0;
} TRISDBITS_t;

typedef struct {
    unsigned char LATD0;
} LATDBITS_t;

typedef struct {
    unsigned char TMR0IF;
    unsigned char TMR0IE;
} INTCONBITS_t;

extern TRISDBITS_t TRISDBits;
extern LATDBITS_t LATDbits;
extern INTCONBITS_t INTCONbits;

extern unsigned int TMR0;
extern unsigned char T0CON;

extern unsigned char TMR0IF;

static inline void di(void) { }
static inline void ei(void) { }

#endif
