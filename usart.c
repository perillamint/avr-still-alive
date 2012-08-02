/*
 * usart.c
 *
 *  Created on: 8 Oct 2011
 *      Author: leraquendee
 *     License: GNU GPL v3
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"

void usart_init()
{
	//TODO: Add baud rate set through argument.
	DDRE = 0xFE;

	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;

	//9600bps @ 16MHz
	//UBRR0H = 0x00;
	//UBRR0L = 0x67;

	//2400bps @ 16MHz
	//UBRR0H = 0x01;
	//UBRR0L = 0xA0;

	//1200bps @ 16MHz
	//UBRR0H = 0x03;
	//UBRR0L = 0xFF;

	//115200bps @ 16MHz
	UBRR0H = 0x00;
	UBRR0L = 0x08;
}

void usart_send_ch(char data)
{
	while (!(UCSR0A & 0x20))
		;

	UDR0 = data;

	UCSR0A |= 0x20;
}

void usart_send_str(char *str)
{
	int i;

	for (i = 0; str[i] != 0x00; i++)
	{
		usart_send_ch(str[i]);
	}
}

void usart_set_cursor(int x, int y)
{
	char buf[5]; //it will not exceed 1E5.

	usart_send_ch(CHAR_ESC);
	usart_send_ch('[');
	sprintf(buf, "%d", y);
	usart_send_str(buf);
	usart_send_ch(';');
	sprintf(buf, "%d", x);
	usart_send_str(buf);
	usart_send_ch('H');
}

//Cursor up/down function. Minus value will move cursor up.
void usart_cursor_ud(int y)
{
	char buf[5]; //it will not exceed 1E5.
	uint8_t reverse = 0;

	if (y < 0)
	{
		reverse = 1;
		y = -y;
	}

	usart_send_ch(CHAR_ESC);
	usart_send_ch('[');
	sprintf(buf, "%d", y);
	usart_send_str(buf);
	if (reverse)
	{
		usart_send_ch('A');
	}
	else
	{
		usart_send_ch('B');
	}
}

//Cursor forward/backward function. Minus value will move cursor backward.
void usart_cursor_fb(int x)
{
	char buf[5]; //it will not exceed 1E5.
	uint8_t reverse = 0;

	if (x < 0)
	{
		reverse = 1;
		x = -x;
	}

	usart_send_ch(CHAR_ESC);
	usart_send_ch('[');
	sprintf(buf, "%d", x);
	usart_send_str(buf);
	if (reverse)
	{
		usart_send_ch('D');
	}
	else
	{
		usart_send_ch('C');
	}
}

void usart_cursor_save()
{
	usart_send_ch(CHAR_ESC);
	usart_send_ch('7');
}

void usart_cursor_restore()
{
	usart_send_ch(CHAR_ESC);
	usart_send_ch('8');
}

void usart_clear_screen()
{
	usart_send_ch(CHAR_ESC);
	usart_send_str("[2J");
}
