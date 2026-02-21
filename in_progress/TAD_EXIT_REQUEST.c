/*
 * File:   TAD_EXIT_REQUEST.h
 * Author: Ari i Marc
 *
 * Created on 15 de febrero de 2026, 17:44
 */

#include "TAD_EXIT_REQUEST.h"
#include <xc.h>
#include "pic18f4321.h"
#include "TAD_TIMER.h"
#include "TAD_CONTROLLER.h"

#define POLSADOR_PREMUT 0
static unsigned char Estat = 0;
static unsigned char Timer = 0;

void Init_ExitRequest(){
    TRISBbits.TRISB0 = 1;
    INTCON2bits.RBPU = 0;
    TI_NewTimer(&Timer);
}

void ER_Motor(){
    switch(Estat){
        case 0:
            if(PORTBbits.RB0 == POLSADOR_PREMUT){
                TI_ResetTics(Timer);
                Estat = 1;
            }
            break;
        case 1:
            if(TI_GetTics(Timer)>=5){
                Estat = 2;
            }
            break;
        case 2:
            if(PORTBbits.RB0 != POLSADOR_PREMUT){
                TI_ResetTics(Timer);
                Estat = 3;
            }
        case 3: 
            if(TI_GetTics(Timer)>=5){
                CO_ExitRequested();
                Estat = 0;
            }
            
    }

}



