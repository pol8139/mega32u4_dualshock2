#include "mega32u4_uart.h"

volatile char c;

ISR(USART1_RX_vect)
{
    while(!(UCSR1A & _BV(RXC1)));
    c = UDR1;
}

void initUart(void)
{
    UBRR1H = (unsigned char)(BAUD_REG >> 8); // set baud rate...
    UBRR1L = (unsigned char)(BAUD_REG & 0xFF); 
    UCSR1A |= _BV(U2X1); // ...to 2000000bps
    UCSR1B = _BV(RXCIE1) | _BV(RXEN1) | _BV(TXEN1); // enable RX interrupt and TX
}

void transmitUart(char data)
{
    while(!(UCSR1A & _BV(UDRE1)));
    UDR1 = data;
}

void transmitUartString(char *data)
{
    char c;
    for(int i = 0; i < STRING_MAX_BYTES; i++) {
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

char recieveUART(void)
{
    char c_recieve = c;
    c = 0xFF;
    return c_recieve;
}