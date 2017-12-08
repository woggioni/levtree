#ifndef LEVTREE_H
#define LEVTREE_H

#include <stdlib.h>
#include "levtree/common.h"
#include "levtree/levnode.h"
#include "levtree/levtree_standing.h"

typedef struct levtree levtree;

struct levtree
{
    index_t maxsize;
    byte_t allocated;
    byte_t torealloc;
    levnode* nodes;
    index_t node_count;
    index_t node_size;
    index_t* entries; //vettore di interi che contiene la posizione di ogni parola nel vettore dei nodi
    index_t entry_count;
    index_t entry_size;
    levtree_standing* standing;
    byte_t (*checker)(char key1, char key2);
    void (*distance_calculator)(levtree* tree, const char* wordkey, index_t wordlen, index_t* path, index_t pathLength, index_t j);
};

void levtree_init(levtree* tree, char **words, index_t words_count);
void levtree_free(levtree* tree);
void levtree_search(levtree* tree, const char* wordkey, index_t n_of_matches);
void levtree_set_algorithm(levtree *tree, levtree_algorithm algo);
void levtree_set_case_sensitive(levtree *tree, byte_t boolean);
void levtree_add_word(levtree* tree, const char* keyword, index_t id);

levtree_result levtree_get_result(levtree* tree, index_t index);

#endif // LEVTREE_H
