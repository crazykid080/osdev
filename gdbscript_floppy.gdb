set disassembly-flavor intel
target remote | qemu-system-i386 -S -gdb stdio -m 32 -fda build/main_floppy.img