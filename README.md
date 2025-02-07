# kOS - A Chaotic OS

This is an attempt at the ***kfs*** project of 42 school. Who knows where it will go.

## Installation
In order to build kOS, a few dependencies are required:

* `grub-common`
* `grub-pc-bin`
* `xorriso`

And to test it:

* `qemu-kvm`
* `qemu-system-x86`

On Debian-based systems, you can install them with:
```sh
sudo apt update && sudo apt install -y grub-common grub-pc-bin xorriso
```
```sh
sudo apt update && sudo apt install -y qemu-kvm qemu-system-x86
```

## Usage
To test the project just `make run`.

## Ressources

* [OSDev: Bare Bones](https://wiki.osdev.org/Bare_Bones)
* [Grub specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
* [Cross-Compiler](https://newos.org/toolchains/i386-elf-7.5.0-Linux-x86_64.tar.xz)
