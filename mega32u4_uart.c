#include "mega32u4_uart.h"

void initUart(void)
{
    UBRR1H = (unsigned char)(BAUD_REG >> 8); // set baud rate...
    UBRR1L = (unsigned char)(BAUD_REG & 0xFF); // ...to 115200bps
    UCSR1B = _BV(RXEN1) | _BV(TXEN1); // enable RX and TX
}

void transmitUart(char data)
{
    while(!(UCSR1A & _BV(UDRE1)));
    UDR1 = data;
}

void transmitUartString(char *data)
{
    char c;
    int i;
    for(i = 0; i < STRING_MAX_BYTES; i++) {
        c = data[i];
        if(c == '\0') {
            break;
        }
        transmitUart(c);
    }
}

void transmitUartStringCRLF(char *data)
{
    transmitUartString(data);
    transmitUart('\r');
    transmitUart('\n');
}