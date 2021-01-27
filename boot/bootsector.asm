;
; A simple boot sector
;
[bits 16]
[org 0x7c00]  ; Tell the assembler to offset the code segment to this address
KERNEL_OFFSET equ 0x1000 
    mov [BOOT_DRIVE], dl    ; BIOS puts boot drive in DL, store this for later
    
    mov bp, 0x8000          ; Set stack to be far away
    mov sp, bp

	call cls				; clear screen

	mov ax, 0x1112
	mov bl, 0
	int 0x10

    mov bx, BOOT_START_OS
    call print_string
	call load_kernel		; load kernel to memory
	call switch_to_pm		; switch to 32 bit protected mode (oh baby!)

%include "boot/print.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/32bit_print.asm"
%include "boot/switch_pm.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET   ; Load to 0x0000(ES):0x9000(BX)
    mov dh, 16              ; Number of sectors to load
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
	call KERNEL_OFFSET
	jmp $

; Global variables
BOOT_DRIVE: db 0            ; BIOS stores our boot drive, placehold now

BOOT_START_OS:
    db 'Booting jOS...',13,10,0;

BOOT_LOAD_KERNEL:
	db 'Loaded Kernel', 13, 10, 0;

; Bootsector padding
    times 510-($-$$) db 0   ; pad with zeros
    dw 0xaa55
