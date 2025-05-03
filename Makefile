CC      = gcc
CFLAGS  = -m32 -fno-stack-protector
LD      = ld
NASM    = nasm
ISO     = pheonixOs.iso

C_SOURCES   = $(shell find kernel -type f -name '*.c')
ASM_SOURCES = $(shell find kernel -type f -name '*.asm')

OBJFILES    = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)

kernelMain: $(OBJFILES)
	$(LD) -m elf_i386 -T link.ld -o $@ $^ -z execstack

%.o: %.asm
	$(NASM) -f elf32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I kernel/include -c $< -o $@

iso: kernelMain
	mkdir -p iso/boot/grub
	cp kernelMain iso/boot/kernel.bin

	echo "menuentry 'pheonixOs' {" >> iso/boot/grub/grub.cfg
	echo "    multiboot /boot/kernel.bin" >> iso/boot/grub/grub.cfg
	echo "}" >> iso/boot/grub/grub.cfg

	grub-mkrescue -o $(ISO) iso

run: iso
	qemu-system-i386 -cdrom $(ISO) 

clean:
	rm -f $(OBJFILES) kernelMain
	rm -f $(ISO)
	rm -rf iso
