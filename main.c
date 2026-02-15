/*
 * File:   main.c
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:33
 */

#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_SERIAL.h"
#include "TAD_TECLAT.h"
#include "TAD_SPEAKER.h"
#include "TAD_EXIT_REQUEST.h"

#pragma config OSC = HS //INTIO2
#pragma config PBADEN = DIG
#pragma config MCLRE = OFF
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

void main(void) {
    ADCON1 = 0xFF;
        
    TI_Init();
    Init_Intensity();
    Init_Leds();
    Init_Serial();
    Init_Teclat();
    Init_Speaker();
    Init_ExitRequest();

    int startMotor = 1;
    
    while(1) {
        //MotorTeclat();
        //motorSerial();
        INT_motor();
        if (startMotor) {
            startMotor = 0;
            INT_start();
        }
    }
    
    return;
}
