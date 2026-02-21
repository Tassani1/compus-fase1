/*
 * File:   TAD_COTROLLER.h
 * Author: ACER
 *
 * Created on 13 de febrero de 2026, 17:17
 */


#ifndef TAD_CONTROLLER_H
#define TAD_CONTROLLER_H

#include <xc.h>

void Init_Controller(void);

//void CO_HallDetected(void);
// Post: Funció que ha de cridar el TAD de Hall quan detecti un canvi d'estat, i obrirà la porta exterior.

void CO_NewKeyPressed(unsigned char key);
// Post: Funció que ha de cridar el TAD de Teclat quan detecti una nova pulsació.

//void CO_ExitRequested(void);
// Post: Funció que ha de cridar el TAD de Exit_Request

void motorController(void);

#endif
