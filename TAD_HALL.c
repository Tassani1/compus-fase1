/*
 * File:   TAD_HALL.c
 * Author: ACER
 *
 * Created on 20 de febrero de 2026, 17:49
 */


#include <xc.h>
#include "TAD_HALL.h"

void Init_Hall(void) {
    TRISCbits.TRISC4 = 1;   // Entrada
}

unsigned char Hall_Detectat(void) {
    return (PORTCbits.RC4 == 0);   // Activo en bajo
}
