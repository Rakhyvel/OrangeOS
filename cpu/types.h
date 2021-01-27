#ifndef TYPES_H
#define TYPES_H
typedef unsigned int   U32;
typedef 		 int   S32;
typedef unsigned short U16;
typedef          short S16;
typedef unsigned char  U8;
typedef 		 char  S8;

#define low_16(address) (U16)((address) & 0xFFFF)
#define high_16(address) (U16)(((address) >> 16) & 0xFFFF)
#endif
