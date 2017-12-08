#ifndef LEVNODE_H
#define LEVNODE_H

#include <levtree/common.h>
typedef struct levnode levnode;

struct levnode
{
    char key;
    index_t id;
    index_t next;
    index_t prev;
    index_t child;
    index_t parent;
    index_t * row;
    byte_t processed;
};

void levnode_init(levnode * node, char key, index_t index);
void levnode_init_copy(levnode * dest, levnode * src);
void levnode_alloc(levnode * node, index_t size);
void levnode_free(levnode * node);

#endif // LEVNODE_H
