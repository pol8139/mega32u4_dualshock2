#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif // not needed when compile but needed when auto-complete of VSCode

#define SS PB6

#define sbi(PORT, BIT) PORT |= _BV(BIT)
#define cbi(PORT, BIT) PORT &= ~_BV(BIT)

#include <avr/io.h>
#include <util/delay.h>

void initSPIMaster(void);
unsigned char transmitAndRecieveSPIbyte(unsigned char);
