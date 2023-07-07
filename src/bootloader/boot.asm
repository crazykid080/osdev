org 0x7C00                  ;Tells compiler to put our code at address 7C00 (The start of the block)
bits 16                     ;Sets instruction bit size as 16 

%define ENDL 0x0D, 0x0A

;
; FAT12 header
;

jmp short start
nop

bdb_oem:                    db 'CRAZY0.1'                   ;8 bytes
bdb_bytes_per_sector:       dw 512
bdb_sectors_per_cluster:    db 1
bdb_reserved_sectors:       dw 1
bdb_fat_count:              db 2
bdb_dir_entries_count:      dw 0E0h
bdb_total_sectors:          dw 2880                         ;2880*512 = 1.44MB
bdb_media_descriptor_type:  db 0F0h                         ;F0 = 3.5" floppy
bdb_sectors_per_fat:        dw 9                            ; 9 sectors/fat
bdb_sectors_per_track:      dw 18
bdb_heads:                  dw 2
bdb_hidden_sectors:         dd 0
dbd_large_sector_count:     dd 0

; extended boot record

ebr_drive_number:           db 0                            ; 0x00 floppy
                            db 0
ebr_signature:              db 29h
ebr_volume_id:              db 03h, 00h, 08h, 00h           ; serial number
ebr_volume_label:           db 'CRAZYKID080'                ; 11 bytes, pad with spaces
ebr_system_id:              db 'FAT12   '                   ; 8 bytes, pad with spaces



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
    mov sp, 0x7c00                          ;Set stack pointer to currrent address. Stack will grow downwards 

    ; read something from disk
    ; BIOS should set DL to drive number
    mov [ebr_drive_number], dl

    mov ax, 1                               ; LBA[1], second sector from disk
    mov cl, 1
    mov bx, 0x7E00                          ; data should be AFTER bootloader 
    call disk_read

    mov si, msg_hello
    call puts

    cli
    hlt                                     ;halts the processor

.halt:
    cli                                     ; disable interrupts to prevent escaping halt state
    hlt


floppy_error:
    mov si, msg_read_failed
    call puts
    jmp wait_key_and_reboot

wait_key_and_reboot:
    mov ah, 0
    int 16h                     ; wait for keypress
    jmp 0FFFFh:0                ; jump to beginning of BIOS. Should force a reboot


;
; Disk routines
;


;
; Converts LBA to CHS address
; Parameters:
;   - ax: LBA address
; Returns:
;   - cx [bits0-5]:sector number
;   - cx [bits 6-15] cylinder
;   - dh head

lba_to_chs:

    push ax
    push dx

    xor dx, dx                              ; dx = 0
    div word [bdb_sectors_per_track]        ; ax = LBA / SectorsPerTrack
                                            ; dx = LBA % SectorsPerTrack

    inc dx                                  ; dx = (LBA % SectorsPerTrack + 1) = sector
    mov cx, dx

    xor dx, dx                              ; dx = 0
    div word [bdb_heads]                    ; ax = (LBA / SectorsPerTrack) / Heads = cylinder
                                            ; dx = (LBA / SectorsPerTrack) % Heads = head

    mov dh, dl                              ; dh = head
    mov ch, al                              ; ch = cylinder (lower 8 bits)
    shl ah, 6
    or cl, ah                               ; put upper 2 bits of cylinder

    pop ax
    mov dl, al                              ; restore DL
    pop ax

    ret


;
; Reads sectors from a disk
; Parameters:
;   - ax: LBA address
;   - cl: number of sectors to read (=<128)
;   - dl: drive number
;   - es:bx memory location to store the read data
;

disk_read:

    push ax                                 ; save registers we will be changing
    push bx
    push cx
    push dx
    push di

    push cx                                 ; Save CL to stack
    call lba_to_chs                         ; Compute CHS
    pop ax                                  ;  AL = Number of sectors to read
    
    mov ah, 02h
    mov di, 3                               ; retry count. This can be 0
    

.retry:
    pusha
    stc                                     ; set carry flag
    int 13h                                 ; if carry flag cleared, sucess
    jnc .done
    
    ; if here, read failed.
    popa                                    ; pop all values  
    call disk_reset

    dec di                                  ; reduce retry count
    test di, di                             ; if not zero...
    jnz .retry                              ; retry

.fail:
    jmp floppy_error

.done:
    popa

    pop di                                 ; restore modified registers
    pop dx
    pop cx
    pop bx
    pop ax
    ret

;
; Resets disk controller
; Paramaters:
;   - dl: drive number
;
disk_reset:
    pusha
    mov ah, 0
    stc
    int 13h
    jc floppy_error
    popa
    ret


msg_hello:              db 'IM VM RICKKKK', ENDL, 0
msg_read_failed:        db 'Read from disk failed', ENDL, 0 

times 510-($-$$) db 0       ;($-$$) = size of program so far. This is the setup to set the last two bytes as needed
dw 0AA55h                   ;Set bytes to declare this as bootableS