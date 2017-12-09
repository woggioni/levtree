#ifndef LEVTREE_H
#define LEVTREE_H

#include <stdlib.h>
#include <stdint.h>

#include <levtree/common.h>
#include <levtree/levnode.h>
#include <levtree/levtree_standing.h>

typedef struct levtree_tree levtree_tree;

struct levtree_tree
{
    index_t maxsize;
    uint8_t allocated;
    uint8_t torealloc;
    levtree_levnode* nodes;
    index_t node_count;
    index_t node_size;
    index_t* entries; //vettore di interi che contiene la posizione di ogni parola nel vettore dei nodi
    index_t entry_count;
    index_t entry_size;
    levtree_standing* standing;
    uint8_t (*checker)(uint32_t key1, uint32_t key2);
    void (*distance_calculator)(levtree_tree* tree, const uint32_t* wordkey, index_t wordlen, index_t* path, index_t pathLength, index_t j);
};

levtree_tree* levtree_tree_init(const char **words, index_t words_count);
void levtree_tree_free(levtree_tree* tree);
void levtree_tree_search(levtree_tree* tree, const char* wordkey, index_t n_of_matches);
void levtree_tree_set_algorithm(levtree_tree *tree, levtree_algorithm algo);
void levtree_tree_set_case_sensitive(levtree_tree *tree, uint8_t boolean);
void levtree_tree_add_word(levtree_tree* tree, const char* keyword, index_t id);

levtree_result levtree_tree_get_result(levtree_tree* tree, index_t index);

#endif // LEVTREE_H
