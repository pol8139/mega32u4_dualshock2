#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif // not needed when compile but needed when auto-complete of VSCode

#define SS PB6
#define LITTLE_DELAY 1 // should be modified if you change SPI clock
#define MAX_NUM_RECIEVE 35
#define FIRST_3_BYTES 3 
#define NUM_ID 1
#define NUM_SS 2

#define sbi(PORT, BIT) PORT |= _BV(BIT)
#define cbi(PORT, BIT) PORT &= ~_BV(BIT)

#include <avr/io.h>
#include <util/delay.h>

void initSPIMaster(void);
unsigned char transmitAndRecieveSPIbyte(unsigned char);
int sendDS2Command(unsigned char *, unsigned char *);
int configModeEnterDS2(unsigned char *);