#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif // not needed when compile but needed when auto-complete of VSCode

#define BAUD_REG 8 // 16MHz, 115200bps(when U2Xn=0)
#define STRING_MAX_BYTES 80

#include <avr/io.h>

void initUart(void);
void transmitUart(char);
void transmitUartString(char *);
void transmitUartStringCRLF(char *);