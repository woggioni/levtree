#ifndef WLEVTREE_H
#define WLEVTREE_H

#include "wlevnode.h"
#include "levtree_standing.h"

typedef struct
{
    index_t maxsize;
    byte_t allocated;
    byte_t torealloc;
    byte_t case_sensitive;
    wlevnode* nodes;
    index_t node_count;
    index_t node_size;
    index_t* entries; //vettore di interi che contiene la posizione di ogni parola nel vettore dei nodi
    index_t entry_count;
    index_t entry_size;
    levtree_standing* standing;
}wlevtree;

void wlevtree_init(wlevtree* tree, wchar_t **words, index_t words_count);
void wlevtree_free(wlevtree* tree);
void wtree_search(wlevtree* tree, const wchar_t* wordkey, index_t n_of_matches);
void wtree_isearch(wlevtree* tree, const wchar_t* wordkey, index_t n_of_matches);
void wtree_search_dl(wlevtree* tree, const wchar_t* wordkey, index_t n_of_matches);
void wtree_isearch_dl(wlevtree* tree, const wchar_t* wordkey, index_t n_of_matches);
void wlevtree_add_word(wlevtree* tree, const wchar_t *keyword, index_t id);
levtree_result wlevtree_get_result(wlevtree* tree, index_t index);

#endif // WLEVTREE_H

