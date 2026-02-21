/*
 * File:   TAD_COTROLLER.h
 * Author: Ari i Marc
 *
 * Created on 13 de febrero de 2026, 17:17
 */


#ifndef TAD_CONTROLLER_H
#define TAD_CONTROLLER_H

#include <xc.h>

void controller_init(void);

//void CO_HallDetected(void);
// Post: Funció que ha de cridar el TAD de Hall quan detecti un canvi d'estat, i obrirà la porta exterior.

void controller_newKeyPressed(unsigned char key);
// Post: Funció que ha de cridar el TAD de Teclat quan detecti una nova pulsació.

void controller_exitRequested(void);
// Post: Funció que ha de cridar el TAD de Exit_Request

void controller_motor(void);

void controller_repChar(char lletra);
#endif
