#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif // not needed when compile but needed when auto-complete of VSCode

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "mega32u4_dualshock2.h"
#include "mega32u4_uart.h"

void itoa8b(int, char *);

int main(void)
{
    initUart();
    transmitUartStringCRLF("init UART done");
    _delay_ms(500);
    initSPIMaster();
    transmitUartStringCRLF("init SPI master done");
    _delay_ms(500);
    char buffer[STRING_MAX_BYTES];
    unsigned char buffer_byte[MAX_NUM_RECIEVE];
    int num_of_bytes;
    while(1) {
        num_of_bytes = readDataAndVibrateDS2(buffer_byte, VIBRATE_SMALL_DISABLE);
        transmitUartString("ID: 0x");
        itoa(buffer_byte[NUM_ID], buffer, 16);
        transmitUartStringCRLF(buffer);
        for(int i = 0; i < num_of_bytes; i++) {
            transmitUartString("0b");
            itoa8b(buffer_byte[i], buffer);
            transmitUartString(buffer);
            sprintf(buffer, " 0x%02x", buffer_byte[i]);
            transmitUartString(buffer);
            sprintf(buffer, " %03d", buffer_byte[i]);
            transmitUartStringCRLF(buffer);
        }
        _delay_ms(100);
    }
    return 0;
}

void itoa8b(int in, char *out)
{
    int i;
    const int digits = 8;
    for(i = 0; i < digits; i++) {
        if(((in >> (digits - 1 - i))) & 0x01 == 0x01) {
            out[i] = '1';
        } else {
            out[i] = '0';
        }
    }
    out[digits] = '\0';
}
