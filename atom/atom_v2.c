#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "atom.h"
#include "assert.h"

#define __MAX_DIGITS__      43
#define TABLE_SIZE          2048
#define SCATTER_LEN         256
#define NELEMS(x)           ((sizeof(x))/(sizeof(x[0])))

char* integer_lut = "0123456789";

/* 
 * atom consisting of atom length, data, and a pointer 
 * to an atom in order to handle hash collisions.
 * if there is a collision, the atom is added to a 
 * linked list at the hashed location in the array
 * 
 * struct size: 24 bytes with x86-64 arch
 */
static struct atom {
    struct atom* link; //* 8 bytes
    int len;           //* 4 bytes, 4 bytes padding
    char data[1];        //* 8 bytes
    

} *buckets[2048];




