CFLAGS ?= -std=c99 -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion \
  -Wformat-truncation -fno-common -Wconversion \
  -g3 -O0 -ffunction-sections -fdata-sections -I. \
  -mcpu=cortex-m4 -mthumb -mfloat-abi=soft $(EXTRA_CFLAGS)

#-Werror

LDFLAGS ?= -Tlinker.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = src/firmware.c 

OBJECTS = $(SOURCES:.c=.o)

RM = rm -f

build: firmware.bin

firmware.elf: $(OBJECTS)
	arm-none-eabi-gcc $(OBJECTS) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	sudo st-flash --reset write $< 0x8000000

clean:
	$(RM) firmware.* $(OBJECTS)

# Rule to compile .c files into .o files
%.o: %.c
	arm-none-eabi-gcc -c $(CFLAGS) $< -o $@
