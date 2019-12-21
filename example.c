#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif // not needed when compile but needed when auto-complete of VSCode

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "mega32u4_dualchosk2.h"
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
    unsigned char buffer_byte[32];
    unsigned char id;
    int controller_type;
    int num_of_bytes;
    int i;
    while(1) {
        cbi(PORTB, SS);
        _delay_us(1);
        transmitAndRecieveSPIbyte(0x01);
        id = transmitAndRecieveSPIbyte(0x42);
        controller_type = id >> 4;
        if((id & 0x0F) == 0x00) {
            num_of_bytes = 32;
        } else {
            num_of_bytes = (id & 0x0F) * 2;
        }
        if(transmitAndRecieveSPIbyte(0x00) != 'Z') {
            _delay_us(1);
            // sbi(PORTB, SS);
            transmitUartStringCRLF("an error occurred (the third byte is not 'Z')");
        }
        for(i = 0; i < num_of_bytes; i++) {
            buffer_byte[i] = transmitAndRecieveSPIbyte(0x00);
        }
        _delay_us(1);
        sbi(PORTB, SS);
        _delay_ms(16);
        transmitUartString("ID: 0x");
        itoa(id, buffer, 16);
        transmitUartStringCRLF(buffer);
        for(i = 0; i < num_of_bytes; i++) {
            transmitUartString("0b");
            itoa8b(buffer_byte[i], buffer);
            transmitUartString(buffer);
            sprintf(buffer, " 0x%02X", buffer_byte[i]);
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
    for(i = 0; i < (digits - 1); i++) {
        if(((in >> (digits - 1 - i))) & 0x01 == 0x01) {
            out[i] = '1';
        } else {
            out[i] = '0';
        }
    }
    out[digits] = '\0';
}
