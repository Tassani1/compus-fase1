/*
 * File:   TAD_TECLAT.h
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:39
 */


#ifndef TAD_TECLAT_H
#define TAD_TECLAT_H

#define SMS_TIMEOUT_TICS   500     // 1 segon / 2 ms = 500 tics (comportament SMS)
#define REBOT_TICS         8       // 16 ms
#define FILES              4
#define COLUMNES           3

#define PREMUT() (PORTBbits.RB1 == 1 || PORTBbits.RB2 == 1 || PORTBbits.RB3 == 1)

void teclat_init(void);

void teclat_motor(void);

char getSMS_0(char fila, char columna);

void teclat_reset(void);

#endif
