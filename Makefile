PROG = example
OBJS = ${PROG}.o mega32u4_dualshock2.o mega32u4_uart.o
MCU = atmega32u4
F_CPU = 16000000UL

ELF = $(PROG).elf
HEX = $(PROG).hex

CC = avr-gcc
CFLAGS = -g -O2 -std=c99 -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS = -g -O2 -mmcu=$(MCU)
OBJCOPY = avr-objcopy
SIZE = avr-size

all: $(HEX)

$(HEX): $(ELF)
	$(OBJCOPY) -j .text -j .data -O ihex $(ELF) $(HEX)
	$(SIZE) --mcu=$(MCU) --format=avr $(ELF)

$(ELF): $(OBJS)
	$(CC) $(CFLAGS) -o $(ELF) $^

clean:
	-$(RM) $(ELF) $(HEX) $(OBJS)
