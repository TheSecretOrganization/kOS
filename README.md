# kOS - A Chaotic OS

This is an attempt at the ***kfs*** project of 42 school. Who knows where it will go.

## Dependencies

In order to build kOS, a few dependencies are required:

An i386-elf cross-compiler. You can find one for Linux [here](https://newos.org/toolchains/i386-elf-7.5.0-Linux-x86_64.tar.xz).\
And some packages:

### Compile

* `grub-common`
* `grub-pc-bin`
* `xorriso`
* `mtools`
* `nasm`

### Run

* `qemu-kvm`
* `qemu-system-x86`

## Installation

### Debian based distributions

```sh
sudo apt update && sudo apt install -y grub-common grub-pc-bin xorriso mtools nasm qemu-kvm qemu-system-x86
```

### Arch

To install the dependencies on Arch, run:

```
sudo pacman -Suy && sudo pacman -S grub mtools libisoburn qemu-desktop libvirt virt-manager nasm
```

### Cross-Compiler

For both **Debian** and **Arch**, you will need a cross-compiler

To install the recommended one, run:

```sh
wget https://newos.org/toolchains/i386-elf-7.5.0-Linux-x86_64.tar.xz
tar -xf i386-elf-7.5.0-Linux-x86_64.tar.xz
rm -rf i386-elf-7.5.0-Linux-x86_64.tar.xz
# You can move the Cross-Compiler anywhere you like, just make sure it's first in your PATH.
mv i386-elf-7.5.0-Linux-x86_64 ~/.local/kos_cross
```

Don't forget to add it to your PATH, either in your `.zshrc` or by typing:

```sh
export PATH="$HOME/.local/kos_cross/bin/:$PATH"
```

### MacOS

No cross-compiler is needed, just run:

```sh
brew install i686-elf-binutils i686-elf-gcc i686-elf-grub qemu nasm
```

## Usage

To run the project use `make run`.

## Development

### Git

To use git hooks under .hooks, type
``

```sh
git config core.hooksPath .hooks
```

### Coding Guidelines

The `.clang-format` file defines the project's formatting rules.
We use `clang-format==20.1.8` as the reference formatter, and CI checks every pull request against this standard.

You can install the formatter by any means you prefer.
However, if you’re feeling lazy, you can run `./tools/format.sh --install` to automatically install the formatter with `pipx`.

## Resources

* [OSDev: Bare Bones](https://wiki.osdev.org/Bare_Bones)
* [Grub specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
* [Cross-Compiler](https://newos.org/toolchains/i386-elf-7.5.0-Linux-x86_64.tar.xz)
