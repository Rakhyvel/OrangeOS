#include "mem.h"

void memory_copy(U8 *source, U8 *dest, int nbytes) {
	for(int i = 0; i < nbytes; i++) {
		dest[i] = source[i];
	}
}

void memory_set(U8* dest, U8 val, U32 length) {
	U8* temp = (U8*)dest;
	for(; length != 0; length--) {
		*temp++ = val;
	}
}

void shift(U8* src, U32 shift) {
	for(int i = (int)src; i >= 0; i--) {
		src[i + shift] = src[i];
	}
}
