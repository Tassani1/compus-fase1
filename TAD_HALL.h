#ifndef TAD_HALL_H
#define TAD_HALL_H

#include <xc.h>

void Hall_Init(void);
void MotorHall(void);
unsigned char Hall_HasNewPulse(void);
unsigned int Hall_GetRPM(void);
unsigned long Hall_GetCount(void);
void Hall_Reset(void);

#endif
