#ifndef WLEVTREE_H
#define WLEVTREE_H

#include "levtree/common.h"
#include "levtree/wlevnode.h"
#include "levtree/levtree_standing.h"

typedef struct wlevtree wlevtree;

struct wlevtree
{
    size_t maxsize;
    byte_t allocated;
    byte_t torealloc;
    wlevnode* nodes;
    size_t node_count;
    size_t node_size;
    size_t* entries; //vettore di interi che contiene la posizione di ogni parola nel vettore dei nodi
    size_t entry_count;
    size_t entry_size;
    levtree_standing* standing;
    byte_t (*checker)(wchar_t key1, wchar_t key2);
    void (*distance_calculator)(wlevtree* tree, const wchar_t* wordkey, size_t wordlen, size_t* path, size_t pathLength, size_t j);
};

wlevtree* wlevtree_init(wchar_t **words, size_t words_count);
void wlevtree_free(wlevtree* tree);
void wlevtree_search(wlevtree* tree, const wchar_t* wordkey, size_t n_of_matches);
void wlevtree_add_word(wlevtree* tree, const wchar_t *keyword, size_t id);
levtree_result wlevtree_get_result(wlevtree* tree, size_t index);
void wlevtree_set_algorithm(wlevtree *tree, levtree_algorithm algo);
void wlevtree_set_case_sensitive(wlevtree *tree, byte_t boolean);

#endif // WLEVTREE_H

