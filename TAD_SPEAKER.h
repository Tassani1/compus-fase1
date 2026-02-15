/*
 * File:   TAD_SPEAKER.h
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#ifndef TAD_SPEAKER_H
#define TAD_SPEAKER_H

void Init_Speaker();
//Post: Inicialitza el port RD3 perquè sigui la sortida del speaker

void SPE_PlayAcuteSound();
// Post: Reprodueix un so agut

void SPE_PlayPressureSound();
// Post: Reprodueix un so greu esperant a la introducció del PIN

void SPE_PlayAlarmSound();
// Post: Reprodueix el so d'alarma

void SPE_StopSound();
// Post: Atura qualsevol so que estigui reproduint-se a l'altaveu

void SPE_Motor(void);
// Post: Motor del TAD_SPEAKER, cridat des del main.

#endif