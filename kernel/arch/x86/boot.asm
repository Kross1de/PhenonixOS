bits 32 ;32 BITS
section .text.start
    ;multiboot spec
    align 4
    dd 0x1BADB002 ;magic
    dd 0x00 ;flags
    dd - (0x1BADB002 + 0x00) ;checksum

section .text
global start
extern kmain
global inb
global outb
global reboot
global shutdown
inb:
    push ebp
    mov ebp, esp
    xor eax, eax
    mov edx, [ebp+8]
    in al, dx
    leave
    ret
outb:
    push ebp
    mov ebp, esp
    mov edx, [ebp+8]
    mov eax, [ebp+12]
    out dx, al
    leave
    ret
reboot:
    cli
    mov eax,0xFFFF
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    jmp 0xFFFF:0x0000
shutdown:
    cli
    hlt
    ret

start:
    cli ;clear interrupts
    mov esp, stack_space ;set stack pointer
    call kmain ;call kernel main
    hlt ;halt the cpu

section .bss
    resb 8192 ;reserve 8kb for stack
stack_space:
