#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "ports.h"
#include "timer.h"
#include "../drivers/keyboard.h"

isr_t interrupt_handlers[256];

// Man
void isr_install() {
    set_idt_gate(0, (U32)isr0);
    set_idt_gate(1, (U32)isr1);
    set_idt_gate(2, (U32)isr2);
    set_idt_gate(3, (U32)isr3);
    set_idt_gate(4, (U32)isr4);
    set_idt_gate(5, (U32)isr5);
    set_idt_gate(6, (U32)isr6);
    set_idt_gate(7, (U32)isr7);
    set_idt_gate(8, (U32)isr8);
    set_idt_gate(9, (U32)isr9);
    set_idt_gate(10, (U32)isr10);
    set_idt_gate(11, (U32)isr11);
    set_idt_gate(12, (U32)isr12);
    set_idt_gate(13, (U32)isr13);
    set_idt_gate(14, (U32)isr14);
    set_idt_gate(15, (U32)isr15);
    set_idt_gate(16, (U32)isr16);
    set_idt_gate(17, (U32)isr17);
    set_idt_gate(18, (U32)isr18);
    set_idt_gate(19, (U32)isr19);
    set_idt_gate(20, (U32)isr20);
    set_idt_gate(21, (U32)isr21);
    set_idt_gate(22, (U32)isr22);
    set_idt_gate(23, (U32)isr23);
    set_idt_gate(24, (U32)isr24);
    set_idt_gate(25, (U32)isr25);
    set_idt_gate(26, (U32)isr26);
    set_idt_gate(27, (U32)isr27);
    set_idt_gate(28, (U32)isr28);
    set_idt_gate(29, (U32)isr29);
    set_idt_gate(30, (U32)isr30);
    set_idt_gate(31, (U32)isr31);

	// Remap the PIC
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0); 

    // Install the IRQs
    set_idt_gate(32, (U32)irq0);
    set_idt_gate(33, (U32)irq1);
    set_idt_gate(34, (U32)irq2);
    set_idt_gate(35, (U32)irq3);
    set_idt_gate(36, (U32)irq4);
    set_idt_gate(37, (U32)irq5);
    set_idt_gate(38, (U32)irq6);
    set_idt_gate(39, (U32)irq7);
    set_idt_gate(40, (U32)irq8);
    set_idt_gate(41, (U32)irq9);
    set_idt_gate(42, (U32)irq10);
    set_idt_gate(43, (U32)irq11);
    set_idt_gate(44, (U32)irq12);
    set_idt_gate(45, (U32)irq13);
    set_idt_gate(46, (U32)irq14);
    set_idt_gate(47, (U32)irq15);

    set_idt(); // Load with ASM
}

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t r) {
	print("Received interrupt: ");
	char s[3];
	int_to_ascii(r.int_no, s);
	print(s);
	print("\n");
	print(exception_messages[r.int_no]);
	print("\n");
}

void register_interrupt_handler(U8 n, isr_t handler) {
	interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
	// Need to send EOI to PICs or they will not send another interrupt
	if (r.int_no >= 40) {
		port_byte_out(0xA0, 0x20); // slave
	}
	port_byte_out(0x20, 0x20); // master

	// Handle the interrupt in a more modular way
	if (interrupt_handlers[r.int_no] != 0) {
		isr_t handler = interrupt_handlers[r.int_no];
		handler(r);
	}
}

void irq_install() {
    /* Enable interruptions */
    asm volatile("sti");
    /* IRQ0: timer */
    init_timer(50);
    /* IRQ1: keyboard */
    init_keyboard();
}
