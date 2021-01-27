#ifndef PORTS_H
#define PORTS_H

#include "../cpu/types.h"

U8 port_byte_in (U16 port);
void port_byte_out (U16 port, U8 data);
U16 port_word_in (U16 port);
void port_word_out (U16 port, U8 data);

#endif
