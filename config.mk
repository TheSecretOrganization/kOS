HOST				= i386-elf
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

OS = $(shell uname -s)
ifeq ($(OS),Darwin)
HOST = i686-elf
GRUB_MKRESCUE = $(HOST)-grub-mkrescue
else
GRUB_MKRESCUE = grub-mkrescue
endif

CC					= $(HOST)-gcc
CPP					= $(HOST)-g++
AS					= nasm
AR					= $(HOST)-ar
GDB					= $(HOST)-gdb
RM					= rm -rf

AFLAGS				= -f elf32

CFLAGS_COMMON		= -std=gnu11 -ffreestanding -O2 -Wall -Wextra -Wshadow -g3 -mno-sse -mno-mmx -mno-80387
MANDATORY_CFLAGS	= -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs
CFLAGS				= $(CFLAGS_COMMON) $(MANDATORY_CFLAGS)

CPPFLAGS_COMMON		=
MANDATORY_CPPFLAGS	= -fno-exceptions -fno-rtti
CPPFLAGS			= $(CPPFLAGS_COMMON) $(MANDATORY_CPPFLAGS)
