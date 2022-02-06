#ifndef DLL_INCLUDED_H
#define DLL_INCLUDED_H

/* Public Function declaration to create and return
 * a new empty doubly linked list 
 */

typedef struct DLL_node_ {
    void*            data;
    struct DLL_node_ *next;
    struct DLL_node_ *prev;
} Node_T;

typedef struct DLL {
    int       size;
    int       (*compare)(const void* d1, const void* d2);
    void      (*destroy)(void* data);
    Node_T*   head;
    Node_T*   tail;
} DLL_T;

extern DLL_T*       DLL_new(void (*destroy)(void* data));
extern void         DLL_insert_next(DLL_T* dll, Node_T* ref, void* app_data);
extern void         DLL_insert_prev(DLL_T* dll, Node_T* ref, void* app_data);
extern int          DLL_remove(DLL_T* dll, Node_T* del_node, void** data);
extern Node_T*      DLL_search(DLL_T* dll, void* app_data);
extern void         DLL_destroy(DLL_T* dll);

#endif /* DLL_INLUDED_H */
