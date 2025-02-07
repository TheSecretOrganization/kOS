# kOS - A Chaotic OS

This is an attempt at the ***kfs*** project of 42 school. Who knows where it will go.

## Installation
In order to build kOS, a few dependencies are required:

* `grub-common`
* `grub-pc-bin`
* `xorriso`
* `mtools`
* `nasm`

And the [Cross-Compiler](https://newos.org/toolchains/i386-elf-7.5.0-Linux-x86_64.tar.xz)

To test it you need:

* `qemu-kvm`
* `qemu-system-x86`

On Debian-based systems, you can install them with:
```sh
sudo apt update && sudo apt install -y grub-common grub-pc-bin xorriso mtools nasm
wget https://newos.org/toolchains/i386-elf-7.5.0-Linux-x86_64.tar.xz
tar -xf i386-elf-7.5.0-Linux-x86_64.tar.xz
rm -rf i386-elf-7.5.0-Linux-x86_64.tar.xz
# You can move the Cross-Compiler anywhere you like, just make sure it's first in your PATH.
mv i386-elf-7.5.0-Linux-x86_64 ~/.local/kos_cross
export PATH="~/.local/kos_cross/bin/:$PATH"
```
```sh
sudo apt update && sudo apt install -y qemu-kvm qemu-system-x86
```

## Usage
To test the project just `make run`.

## Resources
* [OSDev: Bare Bones](https://wiki.osdev.org/Bare_Bones)
* [Grub specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
* [Cross-Compiler](https://newos.org/toolchains/i386-elf-7.5.0-Linux-x86_64.tar.xz)
