/*
 * File:   TAD_SPEAKER.c
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#include "TAD_SPEAKER.h"
#include <xc.h>
#include "pic18f4321.h"
static unsigned char timerSpeaker;

void SPE_PlaySound(int frequency, int duration) {
}

void SPE_StopSound() {
}

void Init_Speaker(){
    TRISDBits.TRISD3 = 0; 
    LATDbits.LATD3 = 0;
    TI_NewTimer(&timerSpeaker);
}

void SPE_PlayAcuteSound(){

}


void SPE_PlayPressureSound(){

}

void SPE_PlayAlarmSound(){
    
}
