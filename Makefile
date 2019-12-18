PROG = main
OBJS = ${PROG}.o
MCU = atmega32u4
F_CPU = 16000000UL

ELF = $(PROG)
IHEX = $(PROG).ihex

CC = avr-gcc
CFLAGS = -g -O2 -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS = -g -O2 -mmcu=$(MCU)
OBJCOPY = avr-objcopy

all: $(IHEX)

$(IHEX): $(ELF)

$(ELF): $(OBJS)

%.ihex: %
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

clean:
	-$(RM) $(ELF) $(IHEX) $(OBJS)
