#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif // not needed when compile but needed when auto-complete of VSCode

#define BAUD_REG 103 // 16MHz, 9600bps(when U2Xn=0)
#define STRING_MAX_BYTES 80

#define sbi(PORT, BIT) PORT |= _BV(BIT)
#define cbi(PORT, BIT) PORT &= ~_BV(BIT)

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

// #define BBLED 5

void initUart(void);
void transmitUart(unsigned char);
void transmitUartString(char *);
void transmitUartStringCRLF(char *);
void initSPIMaster(void);
unsigned char transmitAndRecieveSPIbyte(unsigned char);

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
        cbi(PORTB, PB6);
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
            sbi(PORTB, PB6);
            transmitUartStringCRLF("an error occurred (the third byte is not 'Z')");
        }
        for(i = 0; i < num_of_bytes; i++) {
            buffer_byte[i] = transmitAndRecieveSPIbyte(0x00);
        }
        _delay_us(1);
        sbi(PORTB, PB6);
        _delay_ms(16);
        transmitUartString("Type: 0x");
        itoa(controller_type, buffer, 16);
        transmitUartStringCRLF(buffer);
        for(i = 0; i < num_of_bytes; i++) {
            transmitUartString("0b");
            itoa(buffer_byte[i], buffer, 2);
            transmitUartString(buffer);
            transmitUartString(" 0x");
            itoa(buffer_byte[i], buffer, 16);
            transmitUartStringCRLF(buffer);
        }
        _delay_ms(500);
    }
    return 0;
}

void initUart(void)
{
    UBRR1H = (unsigned char)(BAUD_REG >> 8); // set baud rate...
    UBRR1L = (unsigned char)(BAUD_REG & 0xFF); // ...to 9600bps
    UCSR1B = _BV(RXEN1) | _BV(TXEN1); // enable RX and TX
}

void transmitUart(unsigned char data)
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
    char c;
    int i;
    for(i = 0; i < STRING_MAX_BYTES; i++) {
        c = data[i];
        if(c == '\0') {
            break;
        }
        transmitUart(c);
    }
    transmitUart('\r');
    transmitUart('\n');
}

void initSPIMaster(void)
{
    DDRB = _BV(PB2) | _BV(PB1) | _BV(PB6); // set MOSI, SCK and ~SS to output
    SPCR = _BV(SPE) | _BV(DORD) | _BV(MSTR) | _BV(CPOL) | _BV(CPHA) |  _BV(SPR1); // SPI enable, LSB first, SPI Master, Mode 3, f_OSC/64(250kHz)
    sbi(PORTB, PB6); // set ~SS to 0(HIGH)
}

unsigned char transmitAndRecieveSPIbyte(unsigned char data)
{
    SPDR = data;
    while(!(SPSR & _BV(SPIF)));
    return SPDR;
}