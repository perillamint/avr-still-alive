/*
 * sound.h
 *
 *  Created on: 2011. 10. 27.
 *      Author: leraquendee
 */

#ifndef SOUND_H_
#define SOUND_H_

void sound_gentone_thread(void);
void sound_gentone(int freq, int time);
void sound_gentone_musicbox(const int sheet[][2]);
void sound_play_note(unsigned int note, unsigned int duration);

#define FREQ_CAL(x) (F_CPU/2/N/x)
#define N 8
#define PAUSE 10

#endif /* SOUND_H_ */
