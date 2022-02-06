#include "dll.h"
#include <memory.h>
#include <stdlib.h>
#include <assert.h>

#define T DLL_T
#define MALLOC(y)      (y = malloc(sizeof *(y)))
#define FREE(x)        free(x);


/* get DLL head element */
#define DLL_HEAD(dll)       (dll->head)

/* get DLL tail element */
#define DLL_TAIL(dll)       (dll->tail)

/* get DLL size */
#define DLL_SIZE(dll)       (dll->size)

/* macro to reference next element from elem */
#define DLL_NEXT(elem)       (dll->next)

/* macro reference the previous element from elem */
#define DLL_PREV(elem)       (dll->prev)

/* access data from elem */
#define DLL_DATA(elem)       (elem->data)

/* determine if elem is the element */
#define DLL_IS_HEAD(elem)   (elem->prev == NULL ? 1 : 0)

/* determine if elem is tail element */
#define DLL_IS_TAIL(elem)   (elem->next == NULL ? 1 : 0)


/**
 * Initialize a new doubly linked-list. 
 * The only parameter is a function pointer
 * to a user defined function to destroy data
 * contained in DLL node 
 */

T* DLL_new(void (*destroy)(void* data)) {

    T* dll;
    
    MALLOC(dll);
    dll->size = 0;
    dll->head = NULL;
    dll->tail = NULL;
    return dll;
}


/**
 * Function to insert a new node into the DLL
 * in front of ref
 */
void DLL_insert_next (T *dll, Node_T* ref, void* app_data) {

    assert(dll);
    assert(app_data);
    if (ref == NULL && dll->size != 0) {
        return -1;
    }

    Node_T* new_node;

    if((MALLOC(new_node)) == NULL) {
        return -1;
    }

    new_node->data = (void*) app_data;

    // first handle empty dll
    if (DLL_SIZE(dll) == 0) {
        dll->head = new_node;
        dll->tail = new_node;
        dll->head->next = NULL;
        dll->tail->prev = NULL;

    }

    // now handle non-empty cases
    else {
        
        new_node->next = ref->next;
        new_node->prev = ref;
       
        if (ref->next == NULL) {
            dll->tail = new_node;
        }
        else {
            ref->next->prev = new_node;
        }
        ref->next = new_node;
    }

    dll->size++;

    return 0;
    

}

/**
 * Function to insert a new node into the DLL
 * behind ref
 */

void DLL_insert_prev(T* dll, Node_T* ref, void* app_data) {

    assert(dll);
    assert(app_data);

    Node_T* new_node;

    /* don't allow a reference node when list is empty */
    if (ref == NULL && dll->size != 0) {
        return -1;
    }


    if((MALLOC(new_node)) == NULL) {
        return -1;
    }

    new_node->data = (void*) app_data;

    /* handle empty list */
    if (DLL_SIZE(dll) == 0) {
        dll->head = new_node;
        new_node->next = NULL;
        new_node->prev = NULL;
        dll->tail = new_node;

    }

    /* list is not empty */
    else {
        
        new_node->next = ref;
        new_node->prev = ref->prev;

        if (ref->prev == NULL) {
            dll->head = new_node;
        }
        else {
            ref->prev->next = new_node;
            
        }
        
        ref->prev = new_node;

    }

    dll->size++;

    return 0;
}

/**
 * Function to remove the node directly
 * This returns a pointer to the memory address 
 * of the removed data. Library handles memory cleanup
 */ 
int DLL_remove(T* dll, Node_T* del_node, void** data) {

    assert(dll);
    assert(del_node);
    

    /* return if DLL is empty */
    if (dll->head == NULL) {
        return;
    }

    *data = del_node->data;

    /* head node is node to be deleted */
    if (dll->head == del_node) {
        dll->head = del_node->next;
        del_node->prev = NULL;
    }

    else {
        del_node->prev->next = del_node->next;
        if (del_node->next == NULL) {
            dll->tail = del_node->prev;
        }
        else {
            del_node->next->prev = del_node->prev;
        }
        
    }

    FREE (del_node);

    dll->size--;
    
    return 0;

}

/**
 * empty our linked list and free allocated memory
 * 
 */

void DLL_destroy(T* dll) {

    assert(dll);
    void*   data;
    while (dll->size > 0) {
        if (DLL_remove(dll, dll->tail, (void**) &data) == 0) {
            dll->destroy(data);
        }
    }

    memset(dll, 0, sizeof(dll));
    
    return 0;
}





