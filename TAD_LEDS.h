/*
 * File:   TAD_STATE_OK.h
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#ifndef TAD_STATE_OK_H
#define TAD_STATE_OK_H

#define LED_STATE_OK 0
#define LED_STATE_ALARM 1

void LEDS_EncenLed (char Led);
// Pre: El llum del LED especificat (State OK(0) o Alarm(1)) està apagat.
// Post: Encèn el llum del LED especificat (State OK(0) o Alarm(1)).

void LEDS_ApagaLed (char Led);
// Pre: El llum del LED especificat (State OK(0) o Alarm(1)) està encès.  
// Post: Apaga el llum del LED especificat (State OK(0) o Alarm(1)).

#endif