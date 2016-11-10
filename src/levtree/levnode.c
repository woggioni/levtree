#include <levtree/levnode.h>
#include <stdlib.h>
#include <string.h>


void levnode_init(levnode* node, char key, index_t index)
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

void levnode_init_copy(levnode *dest, levnode* src)
{
    *dest = *src;
    dest->row = NULL;
}

void levnode_free(levnode* node)
{
    free(node->row);
}

void levnode_alloc(levnode* node, index_t size)
{
    node->row = (unsigned*) malloc( sizeof(unsigned)*size );
}


