global _start;
[bits 32]

_start:
    [extern start_kernel] ; Calling point
    call start_kernel ; Calls the C function
    jmp $