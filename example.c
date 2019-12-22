#include <stdio.h>
#include <util/delay.h>

#include "mega32u4_dualshock2.h"
#include "mega32u4_uart.h"

void itoa8b(int, char *);
void printBufferData(unsigned char *, int);

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
        readDataDS2(buffer_byte);
        // if(error) {
        //     break;
        // }
        configModeEnterDS2(buffer_byte);
        queryModelAndModeDS2(buffer_byte);
        if(buffer_byte[NUM_ID] == 0xF3) {
            setModeAndLockDS2(buffer_byte, MODE_ANALOG, LOCK_DISABLE);
            vibrationEnableDS2(buffer_byte);
            configModeExitDS2(buffer_byte);
            configModeEnterDS2(buffer_byte);
            pressureEnableDS2(buffer_byte);
            presTransStartDS2(buffer_byte);
            while(1) {
                num_of_bytes = readDataAndVibrateEXDS2(buffer_byte, VIBRATE_SMALL_DISABLE, VIBRATE_BIG_DISABLE);
                // if(error) {
                //     break;
                // }
                printBufferData(buffer_byte, num_of_bytes);
            }
        } else {
            while(1) {
                num_of_bytes = readDataAndVibrateDS2(buffer_byte, VIBRATE_SMALL_DISABLE);
                // if(error) {
                //     break;
                // }
                printBufferData(buffer_byte, num_of_bytes);
            }
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

void printBufferData(unsigned char *data, int num)
{
    char buffer[STRING_MAX_BYTES];
    for(int i = 0; i < num; i++) {
        transmitUartString("0b");
        itoa8b(data[i], buffer);
        transmitUartString(buffer);
        sprintf(buffer, " 0x%02x", data[i]);
        transmitUartString(buffer);
        sprintf(buffer, " %03d", data[i]);
        transmitUartStringCRLF(buffer);
    }
}
