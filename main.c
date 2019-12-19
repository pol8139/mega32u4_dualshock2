#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif // not needed when compile but needed when auto-complete of VSCode

#define BAUD_REG 8 // 16MHz, 115200bps(when U2Xn=0)
#define STRING_MAX_BYTES 80
#define SS PB6

#define sbi(PORT, BIT) PORT |= _BV(BIT)
#define cbi(PORT, BIT) PORT &= ~_BV(BIT)

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void initUart(void);
void transmitUart(unsigned char);
void transmitUartString(char *);
void transmitUartStringCRLF(char *);
void initSPIMaster(void);
unsigned char transmitAndRecieveSPIbyte(unsigned char);
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
            // itoa(buffer_byte[i], buffer, 2);
            transmitUartString("0b");
            itoa8b(buffer_byte[i], buffer);
            transmitUartString(buffer);
            // transmitUartString(" 0x");
            // itoa(buffer_byte[i], buffer, 16);
            sprintf(buffer, " 0x%02X", buffer_byte[i]);
            transmitUartString(buffer);
            sprintf(buffer, " %03d", buffer_byte[i]);
            transmitUartStringCRLF(buffer);
        }
        _delay_ms(100);
    }
    return 0;
}

void initUart(void)
{
    UBRR1H = (unsigned char)(BAUD_REG >> 8); // set baud rate...
    UBRR1L = (unsigned char)(BAUD_REG & 0xFF); // ...to 115200bps
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
    DDRB = _BV(PB2) | _BV(PB1) | _BV(SS); // set MOSI, SCK and SS to output
    SPCR = _BV(SPE) | _BV(DORD) | _BV(MSTR) | _BV(CPOL) | _BV(CPHA) |  _BV(SPR1) | _BV(SPR0); // SPI enable, LSB first, SPI Master, Mode 3, f_OSC/128(125kHz)
    sbi(PORTB, SS); // set SS to 0(HIGH)
}

unsigned char transmitAndRecieveSPIbyte(unsigned char data)
{
    SPDR = data;
    while(!(SPSR & _BV(SPIF)));
    _delay_us(5);
    return SPDR;
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