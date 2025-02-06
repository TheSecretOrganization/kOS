include make.config

all: $(NAME_ISO)

$(NAME_ISO): $(NAME_BIN) $(GRUB_CFG)
	mkdir -pv $(ISO_DIR)/boot/grub
	cp $(KERNEL)/$(NAME_BIN) $(ISO_DIR)/boot
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	grub-mkrescue -o $(NAME_ISO) $(ISO_DIR)

$(NAME_BIN):
	make -C $(KERNEL) all

clean:
	make -C $(KERNEL) clean
	rm -rf $(ISO_DIR) $(NAME_ISO)

re: clean
	make all

run: all
	qemu-system-i386 -cdrom $(NAME_ISO) -no-reboot
