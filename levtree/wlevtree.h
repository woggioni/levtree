#ifndef WLEVTREE_H
#define WLEVTREE_H

#include "common.h"
#include "wlevnode.h"
#include "levtree_standing.h"

typedef struct wlevtree wlevtree;

struct wlevtree
{
    index_t maxsize;
    byte_t allocated;
    byte_t torealloc;
    wlevnode* nodes;
    index_t node_count;
    index_t node_size;
    index_t* entries; //vettore di interi che contiene la posizione di ogni parola nel vettore dei nodi
    index_t entry_count;
    index_t entry_size;
    levtree_standing* standing;
    byte_t (*checker)(wchar_t key1, wchar_t key2);
    void (*distance_calculator)(wlevtree* tree, const wchar_t* wordkey, index_t wordlen, index_t* path, index_t pathLength, index_t j);
};

void wlevtree_init(wlevtree* tree, wchar_t **words, index_t words_count);
void wlevtree_free(wlevtree* tree);
void wlevtree_search(wlevtree* tree, const wchar_t* wordkey, index_t n_of_matches);
void wlevtree_add_word(wlevtree* tree, const wchar_t *keyword, index_t id);
levtree_result wlevtree_get_result(wlevtree* tree, index_t index);
void wlevtree_set_algorithm(wlevtree *tree, levtree_algorithm algo);
void wlevtree_set_case_sensitive(wlevtree *tree, byte_t boolean);

#endif // WLEVTREE_H

