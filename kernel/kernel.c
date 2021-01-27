#include "kernel.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/timer.h"
#include "../libc/function.h"
#include "../cpu/ports.h"


#define DOUBLE_TOP_RIGHT -69
#define DOUBLE_BOTTOM_RIGHT -68
#define DOUBLE_TOP_LEFT -55
#define DOUBLE_BOTTOM_LEFT -56
#define VERT -70
#define HORIZ -51
#define TRANS -53

void print_borders() {
	char line[81];
	for(int i = 0; i < 80; i++) {
		line[i] = HORIZ;
	}
	line[80] = '\0';
	print_at(line, 0, 0);

	for(int i = 0; i < 80; i++) {
		line[i] = ' ';
	}
	line[80] = '\0';
	print(line);

	for(int i = 0; i < 80; i++) {
		line[i] = (char)(U8)196;
	}
	line[80] = '\0';
	print(line);

	print_at("OrangeOS", 1, 0);
	print_at("> ", 0, 1);
}

void main() {
	isr_install();
	irq_install();
	
	clear_screen();
	print_borders();
}

void user_input(char s[]) {
	if(!strcmp(s, "exit")) {
		print_down("Goodbye!");
		asm ("hlt");
	} else	if(!strcmp(s, "cls")) {
		clear_screen();
		print_borders();
	} else {
		print_down(s);
	}
	print_borders();
	port_byte_out(0xED, 4);
}
