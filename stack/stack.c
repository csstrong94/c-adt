#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

#define T Stack_T
#define _NEW(x) ((x) = malloc(sizeof *(x)))
#define _FREE(ptr) ((void)(mem_free((ptr), \
__FILE__, __LINE__), (ptr) = 0))
#define _IS_BAD_PTR(p) ((uintptr_t) p % 8) // check if pointer addr is 8 byte aligned

static void mem_free(void* ptr, const char* file, int line) {
    if (ptr) 
        free(ptr);
}

struct T {
    
    int count;

    struct node {
        void* data;
        struct node* next;
    } *head; 

    int uid;
};

Stack_T Stack_new(void) {
    T stk;

    _NEW(stk);
    stk->count = 0;
    stk->head = NULL;
    stk->uid = 0xcc55;

    return stk;
}

int Stack_empty(T stk) {
    assert(stk);
    assert(stk->uid == 0xcc55);
    return stk->count == 0;
}

void Stack_push(T stk, void* data) {
    struct node *n;
    
    assert(stk);
    assert(stk->uid == 0xcc55);
    _NEW(n);
    n->data = data;
    n->next = stk->head;
    stk->head = n;
    stk->count++;

}

void* Stack_pop(T stk) {
    void* data;
    struct node *temp;

    assert(stk);
    assert(stk->count > 0);
    assert(stk->uid == 0xcc55);
    
    temp = stk->head;
    stk->head = temp->next;
    
    stk->count--;
    data = temp->data;
    _FREE(temp);
    
    return data;

}

void Stack_free(T* stk) {
    struct node *temp, *next;

    
    assert(stk && !_IS_BAD_PTR(stk));
    assert((*stk)->uid == 0xcc55);
    for (temp = (*stk)->head; temp; temp = next) {
        next = temp->next;
        _FREE(temp);
    }
    for (temp = (*stk)->head; temp; temp = next) {
        next = temp->next;
        _FREE(temp);
    }

    _FREE(*stk);

}


