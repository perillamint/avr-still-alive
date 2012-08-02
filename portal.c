/*
 * portal.c
 *
 *  Created on: 2011. 10. 26.
 *      Author: leraquendee
 */
#include "usart.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr-thread.h>
#include "portal.h"

void portal_draw_lyricbox()
{
	usart_set_cursor(1, 1);

	int i, j;
	for (i = 0; i < 49; i++)
	{
		usart_send_ch('-');
	}

	for (i = 0; i < 29; i++)
	{
		usart_send_ch(CHAR_CR);
		usart_send_ch(CHAR_LF);

		usart_send_ch('|');
		for (j = 0; j < 48; j++)
		{
			usart_send_ch(' ');
		}

		usart_send_ch('|');
	}
	usart_send_ch(CHAR_CR);
	usart_send_ch(CHAR_LF);
	usart_send_ch(CHAR_CR);
	usart_send_ch(CHAR_LF);

	for (i = 0; i < 49; i++)
	{
		usart_send_ch('-');
	}
}

void portal_buildform()
{
	usart_send_ch(CHAR_ESC);
	usart_send_str("[32m");

	usart_clear_screen();

	portal_draw_lyricbox();


	int i, j;
	usart_set_cursor(52, 1);

	for (i = 0; i < 49; i++)
	{
		usart_send_ch('-');
	}

	for (i = 0; i < 12; i++)
	{
		usart_set_cursor(51, i + 2);

		usart_send_ch('|');
		for (j = 0; j < 48; j++)
		{
			usart_send_ch(' ');
		}

		usart_send_ch('|');
	}
	usart_set_cursor(52, i + 2);
	for (i = 0; i < 49; i++)
	{
		usart_send_ch('-');
	}
}

void portal_print_lyric(char *str, int delay)
{
	int i;

	for(i=0; str[i] != 0x00; i++)
	{
		usart_send_ch(str[i]);
		avr_thread_sleep(delay);
	}
	usart_cursor_fb(-i);
	usart_cursor_ud(1);
}

void portal_draw_AA(const unsigned char pic[][42])
{
	//20 Line of ascii art data.
	int i;
	const unsigned char *data;

	usart_cursor_save();
	for (i=0; i<20; i++)
	{
		usart_set_cursor(56,15+i);

		data=pic[i];
		while(pgm_read_byte(data) != 0x00)
		     usart_send_ch(pgm_read_byte(data++));
	}
	usart_cursor_restore();
}
