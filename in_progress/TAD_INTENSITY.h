/*
 * File:   TAD_INTENSITY.h
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#ifndef TAD_INTENSITY_H
#define TAD_INTENSITY_H

void Init_Intensity(void);
// Post: Inicialitza el TAD d'Intensity, deixant-lo preparat per a ser utilitzat.

void INT_start();
// Post: Encèn el llum d'Intensity i comença a incrementar la intensitat gradualment per dos minuts màxim.

void INT_stop();
// Pre: El llum d'Intensity està encesa.
// Post: Apaga el llum d'Intensity.

void INT_motor(void);
// Post: Farà avançar el motor del TAD.

#endif
