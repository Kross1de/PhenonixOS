#ifndef GDT_H
#define GDT_H

#define GDT_ENTRIES 3

struct GdtEntry {
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char  baseMid;
    unsigned char  access;
    unsigned char  gran;
    unsigned char  baseHigh;
} __attribute__((packed));

struct GdtPtr {
    unsigned short limit;
    unsigned int   base; 
} __attribute__((packed));

void gdtInit(void);
void gdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdtFlush(void);

#endif