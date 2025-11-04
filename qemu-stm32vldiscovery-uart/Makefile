CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m3 -mthumb -ffreestanding -Os
LDFLAGS = -nostdlib -Wl,--gc-sections -T linker.ld

all: main.elf

main.elf: main.c startup.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

main.bin: main.elf
	$(OBJCOPY) -O binary $< $@

run: main.elf
	qemu-system-arm -M stm32vldiscovery -nographic \
	 -serial tcp::1234,server,nowait -kernel main.elf

clean:
	rm -f main.elf main.bin
