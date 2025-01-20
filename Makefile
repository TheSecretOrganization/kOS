NAME	= aos.bin

AC	 	= i386-elf-as
CC	 	= i386-elf-gcc
CFLAG	= -std=gnu99 -ffreestanding -O2 -Wall -Wextra

$(NAME): boot.o kernel.o linker.ld
	$(CC) -T linker.ld -o $(NAME) -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

all: $(NAME)

boot.o: boot.s
	$(AC) $^ -o $@

kernel.o: kernel.c
	$(CC) $(CFLAG) -c $^ -o $@

clean:
	rm -rf boot.o kernel.o

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

run: $(NAME)
	qemu-system-i386 -kernel $(NAME)
