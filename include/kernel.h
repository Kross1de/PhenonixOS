#ifndef KERNEL_H
#define KERNEL_H

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE (BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES)

#define COLOR_BLACK 0x00
#define COLOR_BLUE 0x01
#define COLOR_GREEN 0x02
#define COLOR_LIGHTGREEN 0x0A
#define COLOR_CYAN 0x03
#define COLOR_RED 0x04
#define COLOR_LIGHTRED 0x0C
#define COLOR_WHITE 0x07

#define MAX_COMMAND_LENGTH 80

extern unsigned int current_loc;
extern char *vidptr;
extern unsigned int lines;
extern unsigned char current_color;
extern char command_buffer[MAX_COMMAND_LENGTH];
extern int command_index;

void set_color(unsigned char color);
void clear_screen(void);
void print(const char *str);
void print_hex(unsigned int value);
void print_int(int num);
void print_prompt(void);
void update_cursor(void);

int keyboard_ready(void);
char get_key(void);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, unsigned int n); 
void process_command(void);

extern unsigned char inb(unsigned short port);
extern void outb(unsigned short port, unsigned char value);
extern void reboot(void);
extern void shutdown(void);

#endif
