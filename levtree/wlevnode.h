#ifndef WLEVNODE_H
#define WLEVNODE_H
#include <wchar.h>

typedef unsigned index_t;
typedef unsigned char byte_t;

typedef struct wlevnode wlevnode;

struct wlevnode
{
    wchar_t key;
    index_t id;
    index_t next;
    index_t prev;
    index_t child;
    index_t parent;
    index_t* row;
    byte_t processed;
};

void wlevnode_init(wlevnode* node, wchar_t key, index_t index);
void wlevnode_init_copy(wlevnode* dest, wlevnode* src);
void wlevnode_alloc(wlevnode* node, index_t size);
void wlevnode_free(wlevnode* node);

#endif // WLEVNODE_H

