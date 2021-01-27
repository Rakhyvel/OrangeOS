#ifndef SCREEN_H
#define SCREEN_H

// Basic constants
#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x6F
#define RED_ON_WHITE 0xF4

// IO ports
#define VGA_CTRL 0x3D4
#define VGA_DATA 0x3D5

// Some functions
void clear_screen();
void print_at(char *message, int col, int row);
void print(char *message);
void print_backspace();
void print_down(char* message);

#endif
