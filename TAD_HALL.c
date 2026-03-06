/*
 * File:   TAD_HALL.c
 * Author: Ari i Marc
 *
 * Created on 20 de febrero de 2026, 17:49
 */


#include <xc.h>
#include "TAD_HALL.h"

char valor = PORTCbits.RC4;

void hall_init(void) {
    TRISCbits.TRISC4 = 1;   // Entrada
}

unsigned char hall_detectat(void) {

    if (!valor){
        return 1;
    }else {
        return 0;
    }
     // Activo en bajo
}
