include make.config

ALL_SRC := $(shell find . -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" \))

.PHONY: all clean re check-format format

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

check-format:
	@clang-format --dry-run --Werror $(ALL_SRC) || (echo "Formatting issues detected! Run 'make format' to fix." && exit 1)

format:
	@clang-format -i $(ALL_SRC)
