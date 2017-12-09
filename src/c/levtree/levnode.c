#include <levtree/levnode.h>
#include <stdlib.h>
#include <string.h>


void levtree_levnode_init(levtree_levnode* node, uint32_t key, index_t index)
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

void levtree_levnode_init_copy(levtree_levnode *dest, levtree_levnode* src)
{
    *dest = *src;
    dest->row = NULL;
}

void levtree_levnode_free(levtree_levnode* node)
{
    free(node->row);
}

void levtree_levnode_alloc(levtree_levnode* node, index_t size)
{
    node->row = (unsigned*) malloc( sizeof(unsigned)*size );
}


