#include "../../../../include/serial.h"

#define COM1_PORT 0x3F8
#define COM2_PORT 0x2F8
#define COM3_PORT 0x3E8
#define COM4_PORT 0x2E8

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

#define SERIAL_LINE_ENABLE_DLAB         0x80

void serial_init(void) {
    outb(SERIAL_LINE_COMMAND_PORT(COM1_PORT), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(COM1_PORT), 0x03); 
    outb(SERIAL_DATA_PORT(COM1_PORT) + 1, 0x00); 
    outb(SERIAL_LINE_COMMAND_PORT(COM1_PORT), 0x03); 
    outb(SERIAL_FIFO_COMMAND_PORT(COM1_PORT), 0xC7); 
    outb(SERIAL_MODEM_COMMAND_PORT(COM1_PORT), 0x0B); 
}

int serial_is_transmit_empty(void) {
    return inb(SERIAL_LINE_STATUS_PORT(COM1_PORT)) & 0x20;
}

void serial_write_char(char c) {
    while (!serial_is_transmit_empty());
    outb(SERIAL_DATA_PORT(COM1_PORT), c);
}

void serial_write(const char *str) {
    unsigned int i = 0;
    while (str[i] != '\0') {
        serial_write_char(str[i]);
        i++;
    }
}

int serial_received(void) {
    return inb(SERIAL_LINE_STATUS_PORT(COM1_PORT)) & 0x01;
}

char serial_read_char(void) {
    while (!serial_received());
    return inb(SERIAL_DATA_PORT(COM1_PORT));
}

void serial_configure_baud_rate(unsigned short divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(COM1_PORT), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(COM1_PORT), (divisor & 0xFF));
    outb(SERIAL_DATA_PORT(COM1_PORT) + 1, (divisor >> 8));
    outb(SERIAL_LINE_COMMAND_PORT(COM1_PORT), 0x03); 
}

void serial_configure_line(unsigned char config) {
    outb(SERIAL_LINE_COMMAND_PORT(COM1_PORT), config);
}