This is a basic bootloader that provides an interactive menu. Make sure to set boot options to legacy.
1. nasm -f bin -o bootloader.bin bootloader.asm (assemble via NASM or use the already .bin file).
2. Create Bootable Image (Win32 Disk Imager, Cygwin, WSL, or RUFUS for Bootable USBs).
3. Use it on a virtual machine for testing purposes (QEMU, VirtualBox, etc.)
