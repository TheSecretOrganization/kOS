include ../config.mk

SRC_DIR		:= src
OBJ_DIR		:= obj
INCLUDES	:= include ../$(LIBK_DIR)/include
INCLD_FLAG	:= $(addprefix -I , $(INCLUDES))
LIB			:= ../$(LIBK_DIR)/$(LIBK)

SRC_C 		:= $(shell find $(SRC_DIR) -type f -name '*.c')
SRC_S		:= $(shell find $(SRC_DIR) -type f -name '*.asm')
OBJ			:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.o, $(SRC_C))
OBJ			+= $(patsubst $(SRC_DIR)/%.asm, $(OBJ_DIR)/%.asm.o, $(SRC_S))
DEP			:= $(OBJ:.o=.d)

.PHONY: all clean re

all: $(NAME_BIN)

$(NAME_BIN): kos.ld $(OBJ)
	$(CC) -T kos.ld -o $(NAME_BIN) $(CFLAGS) $(INCLD_FLAG) $(OBJ) $(LIB) -lgcc

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -MD $(CFLAGS) $(INCLD_FLAG) -c $< -o $@

$(OBJ_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(@D)
	$(AS) $(AFLAGS) $< -o $@

clean:
	$(RM) $(OBJ_DIR)
	$(RM) $(NAME_BIN)

re: clean
	make all

-include $(DEP)
