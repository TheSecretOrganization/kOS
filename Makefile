NAME		= aos
NAME_BIN	= $(NAME).bin
NAME_ISO	= $(NAME).iso

AC		= nasm
CC		= i386-elf-gcc
AFLAG	= -f elf32
CFLAG	= -std=gnu99 -ffreestanding -O2 -Wall -Wextra

ISO_DIR		= isodir
GRUB_CFG	= grub.cfg
MKRESCUE	= grub-mkrescue

$(NAME_ISO): $(NAME_BIN) $(GRUB_CFG)
	mkdir -pv $(ISO_DIR)/boot/grub
	cp $(NAME_BIN) $(ISO_DIR)/boot
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	$(MKRESCUE) -o $(NAME_ISO) $(ISO_DIR)

$(NAME_BIN): boot.o kernel.o linker.ld
	$(CC) -T linker.ld -o $(NAME_BIN) -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

all: $(NAME_ISO)

boot.o: boot.s
	$(AC) $(AFLAG) $^ -o $@

kernel.o: kernel.c
	$(CC) $(CFLAG) -c $^ -o $@

clean:
	rm -rf boot.o kernel.o $(NAME_BIN) $(ISO_DIR)

fclean: clean
	rm -rf $(NAME_ISO)

re: fclean $(NAME_ISO)

run: $(NAME_ISO)
	qemu-system-i386 -cdrom $(NAME_ISO) -no-reboot
