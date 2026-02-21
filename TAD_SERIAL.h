/*
 * File:   TAD_SERIAL.h
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:38
 */

#ifndef TAD_SERIAL_H
#define TAD_SERIAL_H

#include <xc.h>

#define CERT 1
#define FALS 0



void serial_init(void);

unsigned char serial_getChar(void);

void serial_putChar(char lletra);

unsigned char serial_RXAvail(void);

unsigned char serial_TXAvail(void);

void serial_printaMissatge(const char *missatge);

void serial_motor(void);

void serial_esperaYesONo(void);

#endif
