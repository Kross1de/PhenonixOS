#include "../../include/vfs.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREENSIZE (SCREEN_WIDTH * SCREEN_HEIGHT * 2)

unsigned int current_loc = 0;
char *vidptr = (char*)0xb8000;
unsigned int lines = 0;
unsigned char current_color = COLOR_WHITE;

void set_color(unsigned char color) {
    current_color = color;
}

void scroll_screen(void) {
    for (unsigned int i = 0; i < SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2; i++) {
        vidptr[i] = vidptr[i + SCREEN_WIDTH * 2];
    }
    for (unsigned int i = SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2; i < SCREENSIZE; i += 2) {
        vidptr[i] = ' ';
        vidptr[i + 1] = current_color;
    }
    lines--;
    current_loc -= SCREEN_WIDTH;
}

void clear_screen(void) {
    unsigned int i = 0;
    while (i < SCREENSIZE) {
        vidptr[i++] = ' ';
        vidptr[i++] = current_color;
    }
    current_loc = 0;
    lines = 0;
    update_cursor();
}

void update_cursor(void) {
    unsigned short position = current_loc;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

void print(const char *str) {
    unsigned int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            current_loc += SCREEN_WIDTH - (current_loc % SCREEN_WIDTH);
            lines++;
        } else if (str[i] == '\b') {
            if (current_loc > 0) {
                current_loc--;
                vidptr[current_loc * 2] = ' ';
                vidptr[current_loc * 2 + 1] = current_color;
            }
        } else {
            vidptr[current_loc * 2] = str[i];
            vidptr[current_loc * 2 + 1] = current_color;
            current_loc++;
        }

        if (current_loc >= SCREEN_WIDTH * SCREEN_HEIGHT) {
            scroll_screen();
        }

        if (lines >= SCREEN_HEIGHT) {
            scroll_screen();
        }

        update_cursor();
        i++;
    }
}

void print_hex(unsigned int value) {
    char hex[11] = "0x00000000";
    const char hex_digits[] = "0123456789ABCDEF";

    for (int i = 9; i >= 2; i--) {
        hex[i] = hex_digits[value & 0xF];
        value >>= 4;
    }

    print(hex);
}

void print_int(int num) {
    char buffer[12] = {0}; 
    char result[12] = {0};
    int i = 0, j = 0;
    int is_negative = 0;

    if (num == 0) {
        print("0");
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0 && i < sizeof(buffer) - 1) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative) {
        result[j++] = '-';
    }

    while (i > 0) {
        result[j++] = buffer[--i];
    }
    result[j] = '\0';

    print(result);
}

void print_prompt(void) {
    set_color(COLOR_LIGHTRED);
    print("> ");
    set_color(COLOR_LIGHTGREEN);
}