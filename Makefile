CC = arm-none-eabi-gcc -o
CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -O0 -ffunction-sections -fdata-sections -I. \
			-mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16\

LDFLAGS ?= -T link.ld -nostartfiles -nolibc -specs=C:/msys64/mingw64/arm-none-eabi/lib/thumb/v7e-m+fp/hard/nano.specs -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c \
gpio.c \
rcc.c \
systick.c \
usart.c \
pwr.c \
debug.c \
syscalls.c \
exti.c \
syscfg.c \
nvic.c \
tim.c \

OUTNAME = firmware.elf
	
all: build extract

build:
	$(CC) $(OUTNAME) $(SOURCES) $(CFLAGS) $(LDFLAGS)	# compile all object files 

extract: build
	arm-none-eabi-objcopy -O binary firmware.elf firmware.bin	# extract binary image

#program: build extract
#	st-flash --reset write firmware.bin 0x08000000

clean:
	echo "Removing object files..."
	rm -f *.o
	echo "Removing .elf file..."
	rm -f *.elf
	echo "Removing .bin file..."
	rm -f *.bin
	echo "So Fresh!"

set_path:
	export PATH=$PATH:/c/msys64/mingw64/bin