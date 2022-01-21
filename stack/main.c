#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main(void) {
    Stack_T my_stack = Stack_new();
    int* data;

    for (int i = 0; i < 10000; i++) {
        data = malloc(sizeof(int));
        *data = i;
        Stack_push(my_stack,data);
    }

    while (!Stack_empty(my_stack)) {
        int* popped = (int*) Stack_pop(my_stack);
        printf("%d ", *popped);
        
        // if ((*popped / 100 <= 100) && (*popped % 100 == 0)) {
        //     printf("%d ", *popped);
        // }
    }

    Stack_free(&my_stack);

    return 0;

}