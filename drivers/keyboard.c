#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "screen.h"
#include "../kernel/kernel.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

#define SHIFT_PRESSED 0x2A
#define SHIFT_RELEASED 0xAA

static char key_buffer[256];

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii_upper[] = { '?', '?', '!', '@', '#', '$', '%', '^',     
    '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ':', '\"', '`', '?', '|', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};
const char sc_ascii_lower[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g', 
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_callback(registers_t regs) {
	// PIC leaves scancode in port 0x60
	U8 scancode = port_byte_in(0x60);

	if (scancode == SHIFT_RELEASED) {
		shiftDown = 0;
	}

	if (scancode > SC_MAX) return;
	if(scancode == SHIFT_PRESSED) {
		shiftDown = 1;
	} else if (scancode == BACKSPACE) {
        backspace(key_buffer);
        print_backspace();
	} else if (scancode == ENTER) {
		print("\n");
		user_input(key_buffer);
		key_buffer[0] = '\0';
	} else { 
		char letter = '\0';
		if (shiftDown) {
			letter = sc_ascii_upper[(int)scancode];
		} else {
			letter = sc_ascii_lower[(int)scancode];
		}
		char str[2] = {letter, '\0'};
		append(key_buffer, letter);
		print(str);
	}

	UNUSED(regs);
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, keyboard_callback);
}
