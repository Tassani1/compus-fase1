#include "TAD_HALL.h"
#include "TAD_TIMER.h"

#define HALL_PIN PORTDbits.RD4
#define REBOT_TICS 8          // Si és el mateix temps de rebots que per un polsador es deixa així
#define TICS_PER_MIN 60000   // s'ha de canviar aquest valor

static unsigned char estat;
static unsigned char timerRebot;
static unsigned long pulseCount = 0;
static unsigned int lastPeriod = 0;
static unsigned int rpm = 0;
static unsigned int lastTime = 0;
static unsigned char newPulse = 0;

void Hall_Init(void) {
    TRISDbits.RD4 = 1;     // Entrada
    // INTCON2bits.RBPU = 0; Potser s'ha de canviar de port?
    
    TI_NewTimer(&timerRebot);
    
    estat = 0;
    pulseCount = 0;
    rpm = 0;
    newPulse = 0;
}

void MotorHall(void) {
    switch(estat) {
        case 0: // Esperar flanc
            if(HALL_PIN == 1) { // el nostre va al revés, així que hauria de ser 0, però ens hem d'assegurar que sigui així
                TI_ResetTics(timerRebot);
                estat = 1;
            }
        break;
        
        case 1: // Antirrebots
            if(TI_GetTics(timerRebot) >= REBOT_TICS) {
                if(HALL_PIN == 1) { // Tmb 0
                    unsigned int currentTime = TI_GetTics(0);
                    
                    lastPeriod = currentTime - lastTime;
                    lastTime = currentTime;
                    
                    if(lastPeriod > 0)
                        rpm = TICS_PER_MIN / lastPeriod;
                    
                    pulseCount++;
                    newPulse = 1;
                    
                    estat = 2;
                }
                else {
                    estat = 0;
                }
            }
        break;
        
        case 2: // Esperar que no detecti iman (1)
            if(HALL_PIN == 0) {
                estat = 0;
            }
        break;
    }
}

unsigned char Hall_HasNewPulse(void) {
    if(newPulse) {
        newPulse = 0;
        return 1;
    }
    return 0;
}

unsigned int Hall_GetRPM(void) {
    return rpm;
}

unsigned long Hall_GetCount(void) {
    return pulseCount;
}

void Hall_Reset(void) {
    pulseCount = 0;
    rpm = 0;
}
