#ifndef LEVNODE_H
#define LEVNODE_H

#include <levtree/common.h>
typedef struct levtree_levnode levtree_levnode;

struct levtree_levnode
{
    uint32_t key;
    index_t id;
    index_t next;
    index_t prev;
    index_t child;
    index_t parent;
    index_t * row;
    uint8_t processed;
};

void levtree_levnode_init(levtree_levnode * node, uint32_t key, index_t index);
void levtree_levnode_init_copy(levtree_levnode * dest, levtree_levnode * src);
void levtree_levnode_alloc(levtree_levnode * node, index_t size);
void levtree_levnode_free(levtree_levnode * node);

#endif // LEVNODE_H
