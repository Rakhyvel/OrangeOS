#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"

void memory_copy(U8 *source, U8 *dest, int nbytes);
void memory_set(U8 *dest, U8 val, U32 len);
void shift(U8* src, U32 shift);

#endif
