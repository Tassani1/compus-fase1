/*
 * File:   TAD_CONTROLLER.c
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#include "TAD_CONTROLLER.h"
#include <xc.h>
#include "pic18f4321.h"
#include "TAD_TIMER.h"
#include "TAD_SERIAL.h"
#include "TAD_SPEAKER.h"
#include "TAD_INTENSITY.h"
#include "TAD_STATE_OK.h"
#include "TAD_STATE_ALARM.h"
#include "TAD_EXIT_REQUEST.h"


void CO_HallDetected(void) {}

void CO_NewKeyPressed(unsigned char key) {}

void CO_ExitRequested(void) {}

