#include <stdlib.h>
#include <stdio.h>
#include "atom.h"


int main (void) {
    int i = 77889;
    Atom_int(i);
    Atom_string("chris");
    Atom_string("strong");

    return 0;
}