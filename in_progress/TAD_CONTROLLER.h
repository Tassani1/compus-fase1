/*
 * File:   TAD_CONTROLLER.h
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */


#ifndef TAD_CONTROLLER_H
#define TAD_CONTROLLER_H

void CO_HallDetected(void);
// Post: Funció que ha de cridar el TAD de Hall quan detecti un canvi d'estat, i obrirà la porta exterior.

void CO_NewKeyPressed(unsigned char key);
// Post: Funció que ha de cridar el TAD de Teclat quan detecti una nova pulsació.

void CO_ExitRequested(void);
// Post: Funció que ha de cridar el TAD de Exit_Request

void CO_Motor(void);
// Post: Motor del TAD_CONTROLLER, cridat des del main.

#endif
