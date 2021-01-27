#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../cpu/types.h"

U8 shiftDown;

void init_keyboard();
void print_letter(U8 scancode);

#endif
