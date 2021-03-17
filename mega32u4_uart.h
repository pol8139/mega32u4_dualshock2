#ifndef _MEGA32U4_UART_H_
#define _MEGA32U4_UART_H_

#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif // not needed when compile but needed when auto-complete of VSCode

#define BAUD_REG 0 // 16MHz, 2000000bps(when U2Xn=0)
#define STRING_MAX_BYTES 160

#include <avr/io.h>
#include <avr/interrupt.h>

void initUart(void);
void transmitUart(char);
void transmitUartString(char *);
void transmitUartStringCRLF(char *);
char recieveUART(void);

#endif