include ../config.mk

SRC_DIR		:= src
OBJ_DIR		:= obj
INCLD_DIR	:= include

SRC_C 		:= $(shell find $(SRC_DIR) -type f -name '*.c')
SRC_S		:= $(shell find $(SRC_DIR) -type f -name '*.asm')
OBJ			:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.o, $(SRC_C))
OBJ			+= $(patsubst $(SRC_DIR)/%.asm, $(OBJ_DIR)/%.asm.o, $(SRC_S))
DEP			:= $(OBJ:.o=.d)

.PHONY: all clean re

all: $(LIBK)

$(LIBK): $(OBJ)
	$(AR) rcs $@ $(OBJ)

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -MD $(CFLAGS) -I$(INCLD_DIR) -c $< -o $@

$(OBJ_DIR)/%.asm.o: $(SRC_DIR)/%.asm | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(AS) $(AFLAGS) $< -o $@

clean:
	$(RM) $(OBJ_DIR)
	$(RM) $(LIBK)

re: clean
	make all

-include $(DEP)
