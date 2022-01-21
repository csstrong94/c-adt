#include <stdlib.h>
#include <stdio.h>
#include "atom.h"


int main (void) {
    Atom_int(7789);
    Atom_string("chris");
    Atom_string("123-456");
    Atom_int(98765);
    int i = Atom_length("chris");
    printf("%d\n", i);

    return 0;
}