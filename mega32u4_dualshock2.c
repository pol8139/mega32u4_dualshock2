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
    _delay_us(LITTLE_DELAY * 4);
    return SPDR;
}

int sendDS2Command(unsigned char *transmit, int num_send, unsigned char *recieve)
{
    int i;
    unsigned char transmit_command;
    int num_recieve = MAX_NUM_RECIEVE;
    cbi(PORTB, SS);
    _delay_us(LITTLE_DELAY);
    for(i = 0; i < num_recieve; i++) {
        if(num_send > i) {
            transmit_command = transmit[i];
        } else {
            transmit_command = 0x00;
        }
        recieve[i] = transmitAndRecieveSPIbyte(transmit_command);
        if(i == NUM_ID) {
            num_recieve = FIRST_3_BYTES + (recieve[i] & 0x0F) * 2;
        }
    }
    _delay_us(LITTLE_DELAY);
    sbi(PORTB, SS);
    return num_recieve;
}

