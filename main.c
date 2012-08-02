/*
 * main.c
 *
 *  Created on: 2011. 10. 25.
 *      Author: leraquendee
 */

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr-thread.h>
#include "usart.h"
#include "portal.h"
#include "portal_data.h"
#include "sound.h"

#define SIZEOF_STACK 64

void init()
{
	//Timer overflow every 1 millisecond.
	TCCR0 |= (1 << CS02);
	TCNT0 = 0x06;

	// enable overflow interrupt
	TIMSK |= (1 << TOIE0);

	avr_thread_init();
	// enable global interrupts
	sei();

	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRE = 0xFF;

	//Timer1 PWM set
	// Set the Timer/Counter Control Register
	TCCR3A = (1<<COM3A1)|(1<<COM3A0); // Set OC1A when up counting, clear when down counting
	TCCR3B = (1<<WGM33);              // Phase/Freq-correct PWM, top value = ICR1

	usart_init();
}

void play_note(unsigned int note, unsigned int duration)
{
	// Reset the 16 bit Counter
	TCNT3H = 0;
	TCNT3L = 0;

	// Set the Counter TOP
	ICR3H = (note >> 8) & 0x00FF;
	ICR3L = note & 0x00FF;
	// Turn on the Prescaler
	TCCR3B |= (1 << CS31) | (1 << CS32) | (1 << CS30);
	avr_thread_sleep(duration);

	// Turn off the Prescaler
	TCCR3B &= ~((1 << CS31) | (1 << CS32) | (1 << CS30));
	avr_thread_sleep(PAUSE);
}

int main()
{
	init();

	portal_buildform();

	usart_set_cursor(2, 2);
	portal_print_lyric("Forms FORM-29827281-12:", 10);
	portal_print_lyric("Test Assessment Report", 10);
	usart_cursor_ud(2);
	play_note(7812, 10000);
	portal_print_lyric("This was a triumph.", 100);
	portal_print_lyric("I'm making a note here:", 10);
	portal_print_lyric("HUGE SUCCESS.", 10);
	portal_print_lyric("It's hard to overstate", 10);
	portal_print_lyric("my satisfaction.", 10);
	portal_draw_AA(portal_aperturescience);
	portal_print_lyric("Aperture Science", 100);
	portal_print_lyric("We do what we must", 100);
	portal_print_lyric("because we can.", 100);
	portal_print_lyric("For the good of all of us.", 100);
	portal_print_lyric("Except the ones who are dead.", 100);
	usart_cursor_ud(1);
	portal_print_lyric("But there's no sense crying", 100);
	portal_print_lyric("over every mistake.", 100);
	portal_print_lyric("You just keep on trying", 100);
	portal_print_lyric("till you run out of cake.", 100);
	portal_print_lyric("And the science gets done.", 100);
	portal_print_lyric("And you make a neat gun.", 100);
	portal_print_lyric("For the people who are", 100);
	portal_print_lyric("still alive.", 100);

	return 0;
}

ISR(TIMER0_OVF_vect)
__attribute__ ((naked));

ISR(TIMER0_OVF_vect)
{
	avr_thread_isr_start();
	TCNT0 = 0x06;

	avr_thread_isr_end();
}

