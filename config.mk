TARGET				= i386-elf
ARCH				= i386
NAME				= kos
NAME_BIN			= $(NAME).bin
NAME_ISO			= $(NAME).iso

ROOT_DIR			= .
ISO_DIR				= isodir
KERNEL_DIR			= kernel
LIBK_DIR			= libk
LIBK				= libk.a
GRUB_CFG			= grub.cfg

OS					= $(shell uname -s)
ifeq ($(OS),Darwin)
TARGET				= i686-elf
GRUB_MKRESCUE		= $(TARGET)-grub-mkrescue
else
GRUB_MKRESCUE		= grub-mkrescue
endif

CC					= $(TARGET)-gcc
CPP					= $(TARGET)-g++
AS					= nasm
AR					= $(TARGET)-ar
GDB					= $(TARGET)-gdb
RM					= rm -rf

AFLAGS				= -f elf32

MANDATORY_CFLAGS	= -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs
CFLAGS_COMMON		= -std=gnu11 -ffreestanding -Wall -Wextra -Wshadow -mno-sse -mno-mmx -mno-80387
CFLAGS_RELEASE		= -O2
CFLAGS_DEBUG		= -O0 -g

ifeq ($(DEBUG), 1)
CFLAGS				= $(CFLAGS_COMMON) $(CFLAGS_DEBUG) $(MANDATORY_CFLAGS)
else
CFLAGS				= $(CFLAGS_COMMON) $(CFLAGS_RELEASE) $(MANDATORY_CFLAGS)
endif

CPPFLAGS_COMMON		=
MANDATORY_CPPFLAGS	= -fno-exceptions -fno-rtti
CPPFLAGS			= $(CPPFLAGS_COMMON) $(MANDATORY_CPPFLAGS)
