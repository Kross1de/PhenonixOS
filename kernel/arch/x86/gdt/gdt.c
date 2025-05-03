#include "../../../../include/gdt.h"
#include "../../../../include/kernel.h"

struct GdtEntry gdt[GDT_ENTRIES];
struct GdtPtr gp;

void gdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    gdt[num].baseLow = (base & 0xFFFF);
    gdt[num].baseMid = (base >> 16) & 0xFF;
    gdt[num].baseHigh= (base >> 24) & 0xFF;

    gdt[num].limitLow= (limit & 0xFFFF);
    gdt[num].gran    = ((limit >> 16) & 0x0F);
    gdt[num].gran    |=(gran & 0xF0);
    gdt[num].access  = access; 
}

void gdtInit(void) {
    gp.limit = (sizeof(struct GdtEntry) * GDT_ENTRIES) - 1;
    gp.base = (unsigned int)&gdt;
    gdtSetGate(0, 0, 0,  0, 0);
    gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdtFlush();
    print("GDT Address: ");
    print_hex(gp.base);
    print("\n");
}