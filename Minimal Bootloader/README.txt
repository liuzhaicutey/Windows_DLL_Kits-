This is a *basic* bootloader that could corrupt an MBR. It also has an interactive menu hehe. Make sure to set boot options to legacy BIOS.
1. nasm -f bin -o bootloader.bin bootloader.asm (assemble via NASM or use the already .bin file).
2. Create Bootable Image (Win32 Disk Imager, Cygwin, WSL, or RUFUS for Bootable USBs).
3. Use it on a virtual machine for testing purposes (QEMU, VirtualBox, etc.) I HIGHLY RECOMMEND NOT TO TEST IT ON AN ACTUAL MACHINE •̀ω•́
