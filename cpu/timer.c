#include "timer.h"
#include "isr.h"
#include "../cpu/ports.h"
#include "../libc/function.h"

U32 tick = 0;

static void timer_callback(registers_t regs) {
	tick++;

	UNUSED(regs);
}

void init_timer(U32 freq) {
	// Install the function we just wrote
	register_interrupt_handler(IRQ0, timer_callback);

	U32 divisor = 119180 / freq;
	U8 low = (U8)(divisor & 0xFF);
	U8 high = (U8)((divisor >> 8) & 0xFF);
	
	// Send command
	port_byte_out(0x43, 0x36); // Command port
	port_byte_out(0x40, low);
	port_byte_out(0x40, high);
}
