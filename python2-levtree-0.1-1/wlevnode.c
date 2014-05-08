#include "wlevnode.h"
#include <stdlib.h>
#include <string.h>

void wlevnode_init(wlevnode* node, wchar_t key, index_t index)
{
    node->key = key;
    node->id = index;
    node->next = 0;
    node->parent = 0;
    node->prev = 0;
    node->child = 0;
    node->row = NULL;
    node->processed = 0;
}

void wlevnode_init_copy(wlevnode *dest, wlevnode* src)
{
    *dest = *src;
    dest->row = NULL;
}

void wlevnode_free(wlevnode* node)
{
    free(node->row);
}

void wlevnode_alloc(wlevnode* node, index_t size)
{
    node->row = (unsigned*) malloc( sizeof(unsigned)*size );
}



