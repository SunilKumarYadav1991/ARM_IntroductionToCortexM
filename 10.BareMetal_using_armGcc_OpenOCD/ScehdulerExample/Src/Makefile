CC=arm-none-eabi-gcc
MARCH=cortex-m4
CURRENTDIR := ${CURDIR}
INCDIR=$(CURRENTDIR)\Inc
SRCDIR=$(CURRENTDIR)\Src
OUTPUTDIR=$(CURRENTDIR)\obj
CFLAG= -c -mcpu=$(MARCH) -mthumb -std=gnu11 -Wall -o0 
LDFLAG = -nostdlib -T Stm32_linker.ld -Wl,-Map=application.map

all:main.o led.o task.o stm32startup_code.o application.elf

main.o:main.c
	$(CC)	$(CFLAG)-o $@ $^

led.o:led.c
	$(CC)	$(CFLAG)  -o $@ $^

task.o:task.c
	$(CC)	$(CFLAG)  -o $@ $^

stm32startup_code.o:stm32startup_code.c
	$(CC)	$(CFLAG) -o $@ $^

application.elf: main.o led.o task.o stm32startup_code.o
	$(CC)	$(LDFLAG)  -o $@ $^
clean:
	del *.o *.elf

load:
	openocd -f board/stm32f4discovery.cfg
