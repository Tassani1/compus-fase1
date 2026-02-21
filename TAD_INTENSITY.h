/*
 * File:   TAD_INTENSITY.h
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#ifndef TAD_INTENSITY_H
#define TAD_INTENSITY_H

void intensity_init(void);

void intensity_start(void);
// Post: Enc�n el llum d'Intensity i comen�a a incrementar la intensitat gradualment per dos minuts m�xim.

void intensity_stop(void);
// Pre: El llum d'Intensity est� encesa.
// Post: Apaga el llum d'Intensity.

void intensity_motor(void);
// Post: Far� avan�ar el motor del TAD.

#endif