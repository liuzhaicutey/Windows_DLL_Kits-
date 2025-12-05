[org 0x7C00]

start:
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    call display_window
    mov si, msg_title
    call print_msg

    mov si, msg_menu
    call print_msg

    call get_key_input

    cmp al, '1'
    je load_kernel

    cmp al, '2'
    je exit_bootloader

    cmp al, '3'
    je system_info

    mov si, msg_invalid
    call print_msg
    jmp start

load_kernel:
    mov si, msg_loading_kernel
    call print_msg
    jmp 0x1000

exit_bootloader:
    mov si, msg_exit
    call print_msg
halt:
    jmp halt

system_info:
    mov si, msg_system_info
    call print_msg
    mov si, msg_good_luck
    call print_msg
    jmp start

print_msg:
    mov ah, 0x0E
    mov al, [si]
    int 0x10
    inc si
    cmp al, 0x00
    jne print_msg
    ret

get_key_input:
    mov ah, 0x00
    int 0x16
    ret

display_window:
    mov ah, 0x0F
    mov bh, 0x00
    mov bl, 0x0F
    int 0x10
    mov si, window_top_border
    call print_msg
    mov si, window_side_left
    call print_msg
    mov si, window_bottom_border
    call print_msg
    ret

window_top_border:
    db '+-------------------+', 0x0A, 0x00

window_side_left:
    db '|                   |', 0x0A, 0x00

window_bottom_border:
    db '+-------------------+', 0x0A, 0x00

msg_title:
    db 'Minimal Bootloader', 0x0A, 0x00

msg_menu:
    db '1. Load Kernel', 0x0A, 0x00
    db '2. Exit', 0x0A, 0x00
    db '3. System Info', 0x0A, 0x00
    db 'Choose an option: ', 0x00

msg_invalid:
    db 'Invalid choice, try again.', 0x0A, 0x00

msg_loading_kernel:
    db 'Loading Kernel...', 0x0A, 0x00

msg_exit:
    db 'Exiting bootloader...', 0x0A, 0x00

msg_system_info:
    db 'System Info: x86 Bootloader', 0x0A, 0x00

msg_good_luck:
    db 'Have a nice day! I love you!', 0x0A, 0x00

times 510 - ($ - start) db 0
dw 0xAA55
