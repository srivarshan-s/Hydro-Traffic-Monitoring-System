; load 'dh' sectors from drive 'dl' into ES:BX
disk_load:
    pusha
    push dx

    mov ah, 0x02 
    mov al, dh 
    mov cl, 0x02 
 
    mov ch, 0x00 ; cylinder
    mov dh, 0x00 ; head number


    int 0x13      ; BIOS interrupt
    jc disk_error ; if error 

    pop dx
    cmp al, dh    ; No. of sectors read
    jne sectors_error
    popa
    ret


disk_error:
    mov bx, DISK_ERROR
    call print16
    call print16_nl
    mov dh, ah 
    call print16_hex 
    jmp disk_loop

sectors_error:
    mov bx, SECTORS_ERROR
    call print16

disk_loop:
    jmp $

DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0
