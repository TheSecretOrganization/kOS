include ../make.config

SRC_DIR		:= src
OBJ_DIR		:= obj
INCLD_DIR	:= include

SRC_C 		:= $(shell find $(SRC_DIR) -type f -name '*.c')
SRC_S		:= $(shell find $(SRC_DIR) -type f -name '*.s')
OBJ			:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.o, $(SRC_C))
OBJ			+= $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.s.o, $(SRC_S))
DEP			:= $(OBJ:.o=.d)

all: $(NAME_BIN)

$(NAME_BIN): kos.ld $(OBJ)
	$(CC) -T kos.ld -o $(NAME_BIN) $(CFLAGS) $(OBJ) -lgcc

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -MD $(CFLAGS) -I$(INCLD_DIR) -c $^ -o $@

$(OBJ_DIR)/%.s.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	$(AS) $(AFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ_DIR)
	$(RM) $(NAME_BIN)

re: clean
	make all

-include $(DEP)
