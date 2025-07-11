#!/bin/bash

set -e

usage() {
  echo "Usage: $0 <kernel.bin> [breakpoint]"
}

while getopts ":h" opt; do
  case $opt in
  h)
    usage
    exit 0
    ;;
  \?)
    echo "Invalid option: -$OPTARG"
    exit 1
    ;;
  esac
done

if [[ -z $1 ]]; then
  usage
  exit 1
else
  kernel_bin="$1"
fi

if [[ -z $2 ]]; then
  breakpoint="kernel_main"
else
  breakpoint="$2"
fi

if command -v pwndbg >/dev/null 2>&1; then
  dbg=pwndbg
elif command -v gdb >/dev/null 2>&1; then
  dbg=gdb
else
  echo "Please install pwndbg or gdb"
  exit 1
fi

qemu-system-i386 -kernel "$kernel_bin" -no-reboot -S -s &
$dbg -ex "target remote localhost:1234" -ex "break $breakpoint" -ex "continue" "$kernel_bin"
