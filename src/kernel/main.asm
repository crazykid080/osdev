org 0x0
bits 16                     ;Sets instruction bit size as 16 

%define ENDL 0x0D, 0x0A

;goto start of program
start:
    mov si, msg_hello
    call puts

.halt:
    cli
    hlt


; Will print a string to the screen.
; Params:
;   -ds:si pointer to string
;   -
puts:
    push si
    push ax
    push bx

.loop:
    lodsb                   ; Loads next character in al
    or al, al               ; verify if next character is null
    jz .done

    mov ah, 0x0E
    mov bh, 0
    int 0x10

    jmp .loop

.done:
    pop bx
    pop ax
    pop si
    ret

msg_hello: db 'IM KERNEL RICKKKK', ENDL, 0