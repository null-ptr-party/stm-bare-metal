CC = arm-none-eabi-gcc -o
CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
			-mcpu=cortex-m7 -mthumb -mfloat-abi=hard

LDFLAGS ?= -T link.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c gpio.c rcc.c systick.c usart.c
OUTNAME = firmware.elf
	
all: build extract

build:
	$(CC) $(OUTNAME) $(SOURCES) $(CFLAGS) $(LDFLAGS)	# compile all object files 

extract: build
	arm-none-eabi-objcopy -O binary firmware.elf firmware.bin	# extract binary image

clean :
	echo "Removing object files..."
	rm -f *.o
	echo "Removing .elf file..."
	rm -f *.elf
	echo "Removing .bin file..."
	rm -f *.bin
	echo "So Fresh!"