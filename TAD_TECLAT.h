/*
 * File:   TAD_TECLAT.h
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:39
 */


#ifndef TAD_TECLAT_H
#define TAD_TECLAT_H

#include <xc.h>
#include "TAD_TIMER.h"

#define SMS_TIMEOUT_TICS   500     // 1 segon / 2 ms = 500 tics
#define REBOT_TICS         8       // 16 ms
#define FILES              4
#define COLUMNES           3

void Init_Teclat(void);
void MotorTeclat(void);

char Teclat_GetLastChar(void);

unsigned char Teclat_HasNewChar(void);

void Teclat_Reset(void);

#endif
