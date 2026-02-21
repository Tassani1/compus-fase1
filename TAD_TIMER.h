/*
 * File:   TAD_TIMER.h
 * Author: Ari i Marc
 *
 * Created on 11 de febrero de 2026, 9:50
 */


#ifndef TAD_TIMER_H
#define TAD_TIMER_H

// CONSTANTS
#define TI_FALS 0
#define TI_CERT 1

//La RSI
void RSI_Timer0(void);
    // IMPORTANT! Funci� que ha der ser cridadda des de la RSI, en en cas que TMR0IF==1.
    // La RSI ha de contenir: if (TMR0IF==1) RSI_Timer0();

void timer_init (void);
	// Post: Constructor. �s precondici� global haver cridat aquesta funci� quan es cridi qualsevol altra funci� del TAD

unsigned char timer_newTimer(unsigned char *TimerHandle) ;
	// Post: Retorna TI_CERT en cas que s'hagi creat adequadament un nou timer, i TI_FALS en cas contrati.
    // Posa a *TimerHandle l'identificador de timer assignat, necessari per usar les funcions TI_GetTics i TI_ResetTics.

void timer_resetTics (unsigned char TimerHandle);
	// Pre: Handle ha estat retornat per Ti_NewTimer.
	// Post: Engega la temporitzaci� associada a 'TimerHandle', guardant la referencia temporal en el moment de la crida.

unsigned long timer_getTics (unsigned char TimerHandle);
	// Pre: Handle ha estat retornat per timer_newTimer.
	// Post: Retorna els tics transcorreguts des de la crida a timer_resetTics per al mateix TimerHandle.

void timer_end (void);
	// Post: Destructor del TAD

#endif
