/*
 * File:   TAD_SERIAL.h
 * Author: Ari i Marc
 *
 * Created on 9 de febrero de 2026, 12:38
 */
#ifndef TAD_SERIAL_H
#define TAD_SERIAL_H

/* Inicialització */
void SIO_Init(void);

/* Motor cooperatiu */
void Motor_Serial(void);


unsigned char SIO_RXAvail(void);

unsigned char SIO_GetChar(void);

unsigned char SIO_TXAvail(void);

void SIO_PutChar (unsigned char Valor);

void SIO_WriteMessage(char *missatge);

#endif
