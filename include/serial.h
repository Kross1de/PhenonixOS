#ifndef SERIAL_H
#define SERIAL_H

void serial_init(void);
void serial_write_char(char c);
void serial_write(const char *str);
char serial_read_char(void);
int serial_received(void);
int serial_is_transmit_empty(void);
void serial_configure_baud_rate(unsigned short divisor);
void serial_configure_line(unsigned char config);

#endif