#ifndef WLEVNODE_H
#define WLEVNODE_H
#include <wchar.h>

typedef unsigned index_t;
typedef unsigned char byte_t;

typedef struct wlevnode wlevnode;

struct wlevnode
{
    wchar_t key;
    size_t id;
    size_t next;
    size_t prev;
    size_t child;
    size_t parent;
    size_t* row;
    byte_t processed;
};

void wlevnode_init(wlevnode* node, wchar_t key, size_t index);
void wlevnode_init_copy(wlevnode* dest, wlevnode* src);
void wlevnode_alloc(wlevnode* node, size_t size);
void wlevnode_free(wlevnode* node);

#endif // WLEVNODE_H

