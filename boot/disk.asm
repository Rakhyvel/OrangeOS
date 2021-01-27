; loads DH sectors to ES:BX from drive# DL
disk_load:
    push dx         ; store dx on the stack

    mov ah, 0x02    ; BIOS read sector(s) function
    mov al, dh      ; Read DH number of sectors
    mov ch, 0x00    ; Select cylinder 0
    mov dh, 0x00    ; Select head 0
    mov cl, 0x02    ; Start reading from 2 sector (just after boot sect)
    int 0x13        ; issue BIOS read interrupt

    jc disk_read_error   ; any error from reading

    pop dx          ; restore dx from stack
    cmp dh, al      ; if AL (sectors read) != DH (sectors expected),
    jne disk_sector_error  ;   display error message
    ret

disk_read_error:
    mov bx, DISK_READ_ERROR_MSG
    call print_string
    jmp $

disk_sector_error:
    mov bx, DISK_SECTOR_ERROR_MSG
    call print_string
    jmp $

; Variables
DISK_READ_ERROR_MSG db "Disk read error, carry flag after doing interrput",0
DISK_SECTOR_ERROR_MSG db "Disk read error, incorrect sectors",0
