#include <stdint.h>
#include <stddef.h>
#include "../../../../include/gdt.h"
#include "../../../../include/kernel.h"

// Constants for GDT configuration
#define GDT_ENTRIES         6
#define SEGMENT_BASE        0
#define SEGMENT_LIMIT       0xFFFFF
#define CODE_RX_ACCESS      0x9A  // Present, ring 0, code, executable, readable
#define DATA_RW_ACCESS      0x92  // Present, ring 0, data, writable
#define USER_CODE_ACCESS    0xFA  // Present, ring 3, code, executable, readable
#define USER_DATA_ACCESS    0xF2  // Present, ring 3, data, writable
#define TSS_ACCESS          0x89  // Present, TSS, available
#define GRANULARITY_4K      0xCF  // 4KB granularity, 32-bit segment

// Ensure proper structure packing
#pragma pack(push, 1)

// GDT entry structure
struct GdtEntry {
    uint16_t limitLow;      // Lower 16 bits of segment limit
    uint16_t baseLow;       // Lower 16 bits of base address
    uint8_t  baseMid;       // Middle 8 bits of base address
    uint8_t  access;        // Access byte (type, DPL, present)
    uint8_t  granularity;   // Granularity and upper 4 bits of limit
    uint8_t  baseHigh;      // Upper 8 bits of base address
};

// GDT pointer structure
struct GdtPtr {
    uint16_t limit;         // Size of GDT - 1
    uint32_t base;          // Base address of GDT
};

#pragma pack(pop)

// Global GDT and pointer
struct GdtEntry gdt[GDT_ENTRIES];
struct GdtPtr gp;

/**
 * Sets up a GDT descriptor
 * @param num Entry index in GDT
 * @param base Segment base address
 * @param limit Segment limit
 * @param access Access byte configuration
 * @param gran Granularity and flags
 * @return 0 on success, -1 on invalid parameters
 */
int gdtSetGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // Validate parameters
    if (num >= GDT_ENTRIES) {
        print("GDT: Invalid entry number\n");
        return -1;
    }

    // Set base address
    gdt[num].baseLow  = base & 0xFFFF;
    gdt[num].baseMid  = (base >> 16) & 0xFF;
    gdt[num].baseHigh = (base >> 24) & 0xFF;

    // Set limit and granularity
    gdt[num].limitLow   = limit & 0xFFFF;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);

    // Set access byte
    gdt[num].access = access;

    return 0;
}

/**
 * Initializes the Global Descriptor Table
 * @return 0 on success, -1 on failure
 */
int gdtInit(void) {
    // Set up GDT pointer
    gp.limit = (sizeof(struct GdtEntry) * GDT_ENTRIES) - 1;
    gp.base  = (uint32_t)&gdt;

    // Initialize GDT entries
    if (gdtSetGate(0, 0, 0, 0, 0) < 0) return -1;                    // Null descriptor
    if (gdtSetGate(1, SEGMENT_BASE, SEGMENT_LIMIT, CODE_RX_ACCESS, GRANULARITY_4K) < 0) return -1; // Kernel code
    if (gdtSetGate(2, SEGMENT_BASE, SEGMENT_LIMIT, DATA_RW_ACCESS, GRANULARITY_4K) < 0) return -1; // Kernel data
    if (gdtSetGate(3, SEGMENT_BASE, SEGMENT_LIMIT, USER_CODE_ACCESS, GRANULARITY_4K) < 0) return -1; // User code
    if (gdtSetGate(4, SEGMENT_BASE, SEGMENT_LIMIT, USER_DATA_ACCESS, GRANULARITY_4K) < 0) return -1; // User data
    // Note: Entry 5 reserved for TSS, to be set up separately

    // Load GDT
    gdtFlush();

    // Print debugging information
    print("GDT Initialized at: ");
    print_hex(gp.base);
    print("\nSize: ");
    print_hex(gp.limit + 1);
    print(" bytes\n");

    return 0;
}