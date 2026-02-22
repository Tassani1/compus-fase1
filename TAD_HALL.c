/*
 * File:   TAD_HALL.c
 * Author: Ari i Marc
 *
 * Created on 20 de febrero de 2026, 17:49
 */


#include <xc.h>
#include "TAD_HALL.h"

void hall_init(void) {
    TRISCbits.TRISC4 = 1;   // Entrada
}

unsigned char hall_detectat(void) {
    return (PORTCbits.RC4 == 0);   // Activo en bajo
}
