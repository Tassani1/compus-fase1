/*
 * File:   TAD_SPEAKER.h
 * Author: Ari i Marc
 *
 * Created on 14 de febrero de 2026, 16:12
 */

#ifndef TAD_SPEAKER_H
#define TAD_SPEAKER_H

void SPE_PlaySound(int frequency, int duration);
// Post: Reprodueix un so amb l'altaveu a la freqüència i durada especificades.

void SPE_StopSound();
// Post: Atura qualsevol so que estigui reproduint-se a l'altaveu

#endif