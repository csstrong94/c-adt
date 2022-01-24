#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "atom.h"


int main (void) {

    clock_t tic;
    clock_t toc;
    int a_len;
    double etime;
    int num_elems = 10000;

    tic = clock();
    for (int i = 0; i < num_elems; i++) {
        int j = rand();
        
        Atom_int(j);
        
    }
    toc = clock();
    etime = (double) (toc - tic) / CLOCKS_PER_SEC; 



    Atom_string("chris");
    Atom_string("chris");
    a_len = Atom_length("chris");
    display_dist();
    printf("Elapsed time - %d elements: %f\n", num_elems, etime);

    return 0;    

}


// int main (void) {
//     Atom_int(7789);
//    const char* a = Atom_string("chris");
//     Atom_string("123-456");
//     Atom_int(98765);
//     int i = Atom_length(a);
//     printf("%d\n", i);

//     return 0;
// }