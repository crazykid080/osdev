org 0x7C00                  ;Tells compiler to put our code at address 7C00 (The start of the block)
bits 16                     ;Sets instruction bit size as 16 

%define ENDL 0x0D, 0x0A

;goto start of program
start:
    jmp main

; Will print a string to the screen.
; Params:
;   -ds:si pointer to string
;   -
puts:
    push si
    push ax

.loop:
    lodsb                   ; Loads next character in al
    or al, al               ; verify if next character is null
    jz .done
    mov ah, 0x0e
    mov bh, 0
    int 0x10
    jmp .loop

.done:
    pop ax
    pop si
    ret

main:
    ; setup data segments
    mov ax, 0
    mov ds, ax
    mov es, ax

    ; setup stack
    mov ss, ax
    mov sp, 0x7c00          ;Set stack pointer to currrent address. Stack will grow downwards 

    mov si, msg_hello
    call puts

    hlt                     ;halts the processor

.halt:
    jmp .halt               ;Constantly jumps to itself. While true loop


msg_hello: db 'IM VM RICKKKK', ENDL, 0


times 510-($-$$) db 0       ;($-$$) = size of program so far. This is the setup to set the last two bytes as needed
dw 0AA55h                   ;Set bytes to declare this as bootable