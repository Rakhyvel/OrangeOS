#include "ports.h"

U8 port_byte_in (U16 port) {
    unsigned char result;
	// Run in instruction, "d" is the port, "a" is the result
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(U16 port, U8 data) {
	// send out byte to port
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

U16 port_word_in (U16 port) {
    unsigned char result;
	// Run in instruction, "d" is the port, "a" is the result
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(U16 port, U8 data) {
	// send out byte to port
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
