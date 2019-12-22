#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif // not needed when compile but needed when auto-complete of VSCode

#define SS PB6
#define LITTLE_DELAY 1 // should be modified if you change SPI clock
#define MAX_NUM_RECIEVE 35
#define FIRST_3_BYTES 3 
#define NUM_ID 1
#define NUM_SS 2
#define MODE_ANALOG 0x01
#define MODE_DIGITAL 0x00
#define LOCK_ENABLE 0x03
#define LOCK_DISABLE 0x00
#define VIBRATE_SMALL_ENABLE 1
#define VIBRATE_SMALL_DISABLE 0
#define VIBRATE_BIG_DISABLE 0

#define sbi(PORT, BIT) PORT |= _BV(BIT)
#define cbi(PORT, BIT) PORT &= ~_BV(BIT)

#include <avr/io.h>
#include <util/delay.h>

void initSPIMaster(void);
unsigned char transmitAndRecieveSPIbyte(unsigned char);
int sendDS2Command(unsigned char *, int, unsigned char *);
int configModeEnterDS2(unsigned char *);
int configModeExitDS2(unsigned char *);
int setModeAndLockDS2(unsigned char *, unsigned char, unsigned char);
int vibrationEnableDS2(unsigned char *);
int queryModelAndModeDS2(unsigned char *);
int readDataDS2(unsigned char *);
int readDataAndVibrateDS2(unsigned char *, unsigned char);
int readDataAndVibrateEXDS2(unsigned char *, unsigned char, unsigned char);
int pressureEnableDS2(unsigned char *);
int presTransStartDS2(unsigned char *);
