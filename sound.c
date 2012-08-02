/*
 * sound.c
 *
 *  Created on: 2011. 10. 27.
 *      Author: leraquendee
 */

#include <util/delay.h>
#include <avr/io.h>
#include <avr-thread.h>
#include "sound.h"

#define SIZEOF_STACK 128

avr_thread_context sound_context;
avr_thread_context musicbox_context;
uint8_t sound_thread_stack[SIZEOF_STACK];
uint8_t sound_musicbox_stack[SIZEOF_STACK];

int freq_arg;
int time_arg = 1000;
int sheet_arg[10][2];

void sound_delay(uint16_t delay)
{
	uint16_t i;

	for (i = 0; i < delay; i++)
	{
		__asm__ ("nop");
		__asm__ ("nop");
		__asm__ ("nop");
		__asm__ ("nop");
		__asm__ ("nop");
		__asm__ ("nop");
		__asm__ ("nop");
		__asm__ ("nop");
		__asm__ ("nop");
		__asm__ ("nop");
		/*__asm__ ("nop");
		 __asm__ ("nop");
		 __asm__ ("nop");
		 __asm__ ("nop");
		 __asm__ ("nop");
		 __asm__ ("nop");*/
	}
}

void sound_play_note(unsigned int note, unsigned int duration)
{
	// Reset the 16 bit Counter
	TCNT1H = 0;
	TCNT1L = 0;

	// Set the Counter TOP
	ICR1H = (note >> 8) & 0x00FF;
	ICR1L = note & 0x00FF;
	// Turn on the Prescaler
	TCCR1B |= (1 << CS11) | (1 << CS12) | (1 << CS10);
	avr_thread_sleep(duration);

	// Turn off the Prescaler
	TCCR1B &= ~((1 << CS11) | (1 << CS12) | (1 << CS10));
	avr_thread_sleep(PAUSE);
}

void sound_gentone_thread(void)
{
	if (time_arg == 0)
	{
		avr_thread_sleep(time_arg);
	}

	int i;
	int usec = 1000000 / freq_arg / 2 / 2;
	uint32_t tmp = (uint32_t) time_arg * 1000 / ((uint32_t) usec * 2) / 2;
	int loopcnt = (int) tmp;
	int msec = usec / 1000;
	usec %= 1000;

	for (i = 0; i < loopcnt; i++)
	{
		PORTC = 0x01;
		avr_thread_sleep(msec);
		sound_delay(usec);
		PORTC = 0x00;
		avr_thread_sleep(msec);
		sound_delay(usec);
	}
}

void sound_gentone(int freq, int time)
{
	freq_arg = freq;
	time_arg = time;
	avr_thread_start(&sound_context, sound_gentone_thread, sound_thread_stack,
			SIZEOF_STACK);
}

void sound_gentone_musicbox_thread(void)
{
	int i;
	for (i = 0; sheet_arg[i][0] != -1; i++)
	{
		//PORTB = i;
		if (i == 6)
		{
			avr_thread_stop();
		}
		freq_arg = sheet_arg[i][0];
		time_arg = sheet_arg[i][1];
		sound_gentone_thread();
	}
	//avr_thread_stop();
}

void sound_gentone_musicbox(const int sheet[][2])
{
	int i, j;

	for (i = 0; sheet[i][0] != -1; i++)
	{
		for (j = 0; j < 2; j++)
		{
			sheet_arg[i][j] = sheet[i][j];
		}
	}
	avr_thread_start(&musicbox_context, sound_gentone_musicbox_thread,
			sound_musicbox_stack, SIZEOF_STACK);
}
