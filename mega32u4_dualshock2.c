#include "mega32u4_dualchosk2.h"

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
