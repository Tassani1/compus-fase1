/*
 * File:   TAD_SERIAL.h
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:38
 */

#ifndef TAD_SERIAL_H
#define TAD_SERIAL_H

#include <xc.h>

void Init_Serial(void);
unsigned char Serial_GetChar(void);
void Serial_PutChar(char lletra);
//void motorSerial(void);

#endif
