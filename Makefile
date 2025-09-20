include config.mk

ALL_SRC		:= $(shell find . -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" \))
NAME_BIN	:= $(KERNEL_DIR)/$(NAME_BIN)
LIBK		:= $(LIBK_DIR)/$(LIBK)
DEBUG_SH	:= $(TOOLS_DIR)/debug.sh
FORMAT_SH	:= $(TOOLS_DIR)/format.sh

.PHONY: all compile clean re check-format format debug

all: $(NAME_ISO)

$(NAME_ISO): $(GRUB_CFG) compile $(LIBK)
	mkdir -pv $(ISO_DIR)/boot/grub
	cp $(NAME_BIN) $(ISO_DIR)/boot
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	$(GRUB_MKRESCUE) -o $(NAME_ISO) $(ISO_DIR)

compile:
	make -C $(LIBK_DIR) all
	make -C $(KERNEL_DIR) all

clean:
	make -C $(LIBK_DIR) clean
	make -C $(KERNEL_DIR) clean
	rm -rf $(ISO_DIR) $(NAME_ISO)

re: clean
	make all

run: all
	qemu-system-i386 -cdrom $(NAME_ISO) -no-reboot

check-format: $(FORMAT_SH)
	@$(FORMAT_SH) $(ARGS) --dry-run --Werror $(ALL_SRC) || (echo "Formatting issues detected! Run 'make format' to fix." && exit 1)

format: $(FORMAT_SH)
	@$(FORMAT_SH) $(ARGS) -i $(ALL_SRC)

debug: $(DEBUG_SH) compile
	@./$(DEBUG_SH) $(ARGS)
