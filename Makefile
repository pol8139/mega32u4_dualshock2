PROG = example
OBJS = ${PROG}.o mega32u4_dualshock2.o mega32u4_uart.o
MCU = atmega32u4
F_CPU = 16000000UL

ELF = $(PROG)
HEX = $(PROG).hex
# HEX = example.hex

CC = avr-gcc
CFLAGS = -g -O2 -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS = -g -O2 -mmcu=$(MCU)
OBJCOPY = avr-objcopy

all: $(HEX)

$(HEX): $(ELF)

$(ELF): $(OBJS)

%.hex: %
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

clean:
	-$(RM) $(ELF) $(HEX) $(OBJS)
