#ifndef LEVTREE_H
#define LEVTREE_H

#include <stdlib.h>
#include "levnode.h"
#include "levtree_standing.h"


typedef struct
{
    index_t maxsize;
    byte_t allocated;
    byte_t torealloc;
    byte_t case_sensitive;
    levnode* nodes;
    index_t node_count;
    index_t node_size;
    index_t* entries; //vettore di interi che contiene la posizione di ogni parola nel vettore dei nodi
    index_t entry_count;
    index_t entry_size;
    levtree_standing* standing;
}levtree;

void levtree_init(levtree* tree, char **words, index_t words_count);
void levtree_free(levtree* tree);
void levtree_search(levtree* tree, const char* wordkey, index_t n_of_matches);
void levtree_add_word(levtree* tree, const char *keyword, index_t id);
levtree_result levtree_get_result(levtree* tree, index_t index);

#endif // LEVTREE_H
