/*
 * usart.h
 *
 *  Created on: 8 Oct 2011
 *      Author: leraquendee
 *     License: GNU GPL v3
 */

#ifndef USART_H_
#define USART_H_

#define CHAR_ESC 0x1B
#define CHAR_CR 0x0D
#define CHAR_LF 0x0A
#define CHAR_BEL 0x07

void usart_init();
void usart_send_ch(char data);
void usart_send_str(char *str);
void usart_set_cursor(int x, int y);
void usart_cursor_ud (int y);
void usart_cursor_fb (int x);
void usart_cursor_save();
void usart_cursor_restore();
void usart_clear_screen();

#endif /* USART_H_ */
