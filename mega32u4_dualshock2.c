#include "mega32u4_dualshock2.h"

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
    unsigned char transmit_command;
    int num_recieve = MAX_NUM_RECIEVE;
    cbi(PORTB, SS);
    _delay_us(LITTLE_DELAY);
    for(int i = 0; i < num_recieve; i++) {
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

int configModeEnterDS2(unsigned char *recieve)
{
    static unsigned char config_mode_enter[] = {0x01, 0x43, 0x00, 0x01, 0x00};
    return sendDS2Command(config_mode_enter, sizeof(config_mode_enter), recieve);
}

int configModeExitDS2(unsigned char *recieve)
{
    static unsigned char config_mode_exit[] = {0x01, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    return sendDS2Command(config_mode_exit, sizeof(config_mode_exit), recieve);
}

int setModeAndLockDS2(unsigned char *recieve, unsigned char mode, unsigned char lock)
{
    static unsigned char set_mode_and_lock[] = {0x01, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    set_mode_and_lock[3] = mode;
    set_mode_and_lock[4] = lock;
    return sendDS2Command(set_mode_and_lock, sizeof(set_mode_and_lock), recieve);
}

int vibrationEnableDS2(unsigned char *recieve)
{
    static unsigned char vibration_enable[] = {0x01, 0x4d, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff};
    return sendDS2Command(vibration_enable, sizeof(vibration_enable), recieve);   
}

int queryModelAndModeDS2(unsigned char *recieve)
{
    static unsigned char query_model_and_mode[] = {0x01, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    return sendDS2Command(query_model_and_mode, sizeof(query_model_and_mode), recieve);  
}

int readDataDS2(unsigned char *recieve)
{
    static unsigned char read_data[] = {0x01, 0x42, 0x00, 0x00, 0x00};
    return sendDS2Command(read_data, sizeof(read_data), recieve);
}

int readDataAndVibrateDS2(unsigned char *recieve, unsigned char vibrate)
{
    static unsigned char read_data_and_vibrate[] = {0x01, 0x42, 0x00, 0x00, 0x00};
    if(vibrate == VIBRATE_SMALL_ENABLE) {
        read_data_and_vibrate[3] = 0x41;
        read_data_and_vibrate[4] = 0x41;
    } else {
        read_data_and_vibrate[3] = 0x00;
        read_data_and_vibrate[4] = 0x00;
    }
    return sendDS2Command(read_data_and_vibrate, sizeof(read_data_and_vibrate), recieve);
}

int readDataAndVibrateEXDS2(unsigned char *recieve, unsigned char vibrate_small, unsigned char vibrate_big)
{
    static unsigned char read_data_and_vibrate_ex[] = {0x01, 0x42, 0x00, 0x00, 0x00};
    if(vibrate_small == VIBRATE_SMALL_ENABLE) {
        read_data_and_vibrate_ex[3] = 0x01;
    } else {
        read_data_and_vibrate_ex[3] = 0x00;
    }
    read_data_and_vibrate_ex[4] = vibrate_big;
    return sendDS2Command(read_data_and_vibrate_ex, sizeof(read_data_and_vibrate_ex), recieve);
}

int pressureEnableDS2(unsigned char *recieve)
{
    static unsigned char pressure_enable[] = {0x01, 0x4f, 0x00, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00};
    return sendDS2Command(pressure_enable, sizeof(pressure_enable), recieve);
}

int presTransStartDS2(unsigned char *recieve)
{
    static unsigned char pres_trans_start[] = {0x01, 0x43, 0x00, 0x00, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a};
    return sendDS2Command(pres_trans_start, sizeof(pres_trans_start), recieve);
}
