#ifndef _ATOM_INCLUDED
#define _ATOM_INCLUDED

#include <stdint.h>

extern        int     Atom_length(const char* str);
extern  const char*   Atom_new(const char* str, uint32_t len);
extern  const char*   Atom_string(const char* str);
extern  const char*   Atom_int(long n); 


#endif