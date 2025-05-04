#ifndef GDT_H
#define GDT_H

#include <stdint.h>

int gdtInit(void);
int gdtSetGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void gdtFlush(void);

#endif