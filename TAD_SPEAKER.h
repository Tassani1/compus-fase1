/*
 * File:   TAD_SPEAKER.h
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#ifndef TAD_SPEAKER_H
#define TAD_SPEAKER_H

void speaker_init();
//Post: Inicialitza el port RD3 perquè sigui la sortida del speaker

void speaker_playAcuteSound();
// Post: Reprodueix un so agut

void speaker_playPressureSound();
// Post: Reprodueix un so greu esperant a la introducció del PIN

void speaker_playAlarmSound();
// Post: Reprodueix el so d'alarma

void speaker_stopSound();
// Post: Atura qualsevol so que estigui reproduint-se a l'altaveu

void speaker_motor(void);
// Post: Motor del TAD_SPEAKER, cridat des del main.

#endif