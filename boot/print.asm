; BX stores a pointer to begining of null-terminated string
print_string:
    pusha                   ; pushes registers to stack
_push_string_loop:
    mov al, 0               ; load 0 with ax to
    cmp al, [bx]            ; compare 0 with bx
    je  _push_string_end    ; if it's 0, jump to end
    mov ah, 0x0e            ; int 10/ah = 0eh -> scrolling teletype BIOS routine
    mov al, [bx]            ; move value at bx to ax
    int 0x10                ; call bios to print
    add bx, 1               ; increment bx
    jmp _push_string_loop   ; jump back to loop
_push_string_end:
    popa
    ret

cls:
    pusha
    mov ah, 0x00
    mov al, 0x03            ; text mode 80x25 16 colors
    int 0x10
    popa
    ret
