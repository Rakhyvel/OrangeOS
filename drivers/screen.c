#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"
#include "../libc/string.h"

// declarations of private functions
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char color);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

// public screen function
void clear_screen() {
	int screen_size = MAX_COLS * MAX_ROWS;
	char* vga = (char*) VIDEO_ADDRESS;
	
	for(int i = 0; i < screen_size; i++) {
		vga[i*2] = ' ';
		vga[i*2+1] = WHITE_ON_BLACK;
	}
	set_cursor_offset(get_offset(0, 0));
}

void print_at(char* message, int col, int row) {
	int offset;
	if (col >= 0 && row >=0) {
		offset = get_offset(col, row);
	} else { 
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
	
	int i = 0;
	while (message[i] != '\0') {
		offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void print(char* message) {
	print_at(message, -1, -1);
}

void print_down(char* message) {
	shift((U8*)get_offset(0, 3) + VIDEO_ADDRESS, 80 * 2);
	for(int i = 0; i < 80; i++) {
		print_char(' ', i, 3, 0x60);
	}
	print_at(message, 0, 3);
}

void print_backspace() {
    int offset = get_cursor_offset() - 2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(0x08, col, row, WHITE_ON_BLACK);
}


// private function implementation
int get_cursor_offset() {
	port_byte_out(VGA_CTRL, 14);
	int offset = port_byte_in(VGA_DATA) << 8;
	port_byte_out(VGA_CTRL, 15);
	offset += port_byte_in(VGA_DATA);
	return offset * 2;
}

void set_cursor_offset(int offset) {
	offset /= 2;
	port_byte_out(VGA_CTRL, 14);
	port_byte_out(VGA_DATA, (unsigned char) (offset >> 8));
	port_byte_out(VGA_CTRL, 15);
	port_byte_out(VGA_DATA, (unsigned char) (offset & 255));
}

int print_char(char c, int col, int row, char attr) {
    U8 *vidmem = (U8*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else if (c == 0x08) { /* Backspace */
        vidmem[offset] = ' ';
        vidmem[offset+1] = attr;
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++) 
            memory_copy((U8*)(get_offset(0, i) + VIDEO_ADDRESS),
                        (U8*)(get_offset(0, i-1) + VIDEO_ADDRESS),
                        MAX_COLS * 2);

        /* Blank last line */
        char *last_line = (char*) (get_offset(0, MAX_ROWS-1) + (U8*) VIDEO_ADDRESS);
        for (i = 0; i < MAX_COLS; i++) {
			last_line[i*2] = 0;
			last_line[i*2+1] = WHITE_ON_BLACK;
		}

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

int get_offset(int col, int row) {
	return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset) {
	return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) {
	return (offset - (get_offset_row(offset) * 2 * MAX_COLS))/2;
}
