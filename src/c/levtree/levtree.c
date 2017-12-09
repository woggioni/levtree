#include <string.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <wctype.h>

#include <levtree/levtree.h>

#define min(x,y) ((x) < (y) ? (x) : (y))
#define min3(a,b,c) ((a)< (b) ? min((a),(c)) : min((b),(c)))
#define min4(a,b,c,d) ((a)< (b) ? min3((a),(c),(d)) : min3((b),(c),(d)))

static inline uint8_t utf8_char_size(const uint8_t* cursor)
{
    if((*cursor & 0x80) == 0)
        return 1;
    else if((*cursor & 0xc0) == 0xc0)
        return 2;
    else if((*cursor & 0xc0) == 0xc0)
        return 3;
    else if((*cursor & 0xc0) == 0xc0)
        return 4;
    fprintf(stderr, "Unrecognized charachter %#02x\n", *cursor);
    return 1;
}

static inline uint32_t read_utf8_char(const char* cursor, uint8_t* size)
{
    const uint8_t *ptr = (const uint8_t *) cursor;
    uint8_t sz = utf8_char_size(ptr);
    if(size) *size = sz;
    uint32_t result = 0;
    for(uint8_t i=0; i<sz; i++)
    {
        result <<= 8;
        result |= ptr[i];
    }
    return result;
}

static inline uint8_t case_insensitive_checker(uint32_t k1, uint32_t k2)
{
    return towlower(k1) == towlower(k2);
}

static inline uint8_t case_sensitive_checker(uint32_t k1, uint32_t k2)
{
    return k1 == k2;
}

static inline void levtree_tree_alloc_rows(levtree_tree * tree, index_t newsize)
{
    index_t i;

    for(i = 0; i < tree->node_count; i++)
    {
        tree->nodes[i].row = (index_t *) malloc(newsize * sizeof(index_t));
    }
}

static inline void levtree_tree_realloc_rows(levtree_tree * tree, index_t newsize)
{
    index_t i;

    for(i = 0; i < tree->node_count; i++)
    {
        if(tree->nodes[i].row)
        {
            tree->nodes[i].row = (index_t *) realloc(tree->nodes[i].row, newsize * sizeof(index_t));
        }
        else
        {
            tree->nodes[i].row = (index_t *) malloc(newsize * sizeof(index_t));
        }
    }
}

static inline void levtree_tree_delete_rows(levtree_tree * tree)
{
    index_t i;

    for(i = 0; i < tree->node_count; i++)
    {
        if(tree->nodes[i].row)
        {
            free(tree->nodes[i].row);
        }
    }
}

levtree_result levtree_tree_get_result(levtree_tree * tree, index_t pos)
{
    pos = tree->standing->count - pos - 1;
    levtree_result * res = tree->standing->bottom;
    index_t i;

    for(i = 0; i < tree->standing->count; i++)
    {
        if(i == pos)
        {
            break;
        }

        res = res->next;
    }

    return *res;
}

static inline void levtree_tree_add_node(levtree_tree * tree, uint32_t key, index_t index, index_t parent, index_t prev)
{
    tree->node_count++;

    if(tree->node_count >= tree->node_size)
    {
        tree->node_size *= 2;
        tree->nodes = (levtree_levnode *) realloc(tree->nodes, tree->node_size * sizeof(levtree_levnode));
    }

    levtree_levnode * node = &tree->nodes[tree->node_count - 1];

    if(key)
    {
        levtree_levnode_init(node, key, 0);
    }
    else
    {
        levtree_levnode_init(node, key, index);
    }

    node->parent = parent;
    node->prev = prev;

    if(!prev)
    {
        tree->nodes[parent].child = tree->node_count - 1;
    }
    else
    {
        tree->nodes[prev].next = tree->node_count - 1;
    }
}


void levtree_tree_add_word(levtree_tree * tree, const char *keyword, index_t id)
{
    if(tree->allocated)
    {
        levtree_tree_delete_rows(tree);
        tree->allocated = 0;
    }

    index_t initial_nodes = tree->node_count;
    index_t tnode = 0, cnode, nnode;
    index_t maxsize = strlen((const char*) keyword) + 1;

    if(maxsize > tree->maxsize)
    {
        tree->maxsize = maxsize;

        if(tree->allocated)
        {
            tree->torealloc = 1;
        }
    }

    const char *ptr = keyword;
    while(1)
    {
        uint8_t size;
        uint32_t key = read_utf8_char(ptr, &size);
        ptr += size;
        nnode = 0;
        cnode = tree->nodes[tnode].child;

        if(cnode)
        {
            if(tree->nodes[cnode].key == key)
            {
                tnode = cnode;
                goto next;
            }
            else
            {
                nnode = tree->nodes[cnode].next;

                while(nnode)
                {
                    if(tree->nodes[nnode].key == key)
                    {
                        tnode = nnode;
                        goto next;
                    }

                    cnode = nnode;
                    nnode = tree->nodes[nnode].next;
                }
            }
        }

        levtree_tree_add_node(tree, key, id, tnode, cnode);
        tnode = tree->node_count - 1;
        next:
        if(!key) break;
    }

    if(tree->node_count > initial_nodes)
    {
        tree->entry_count++;
        tree->entries[tree->entry_count - 1] = tree->node_count - 1;

        if(tree->entry_count >= tree->entry_size)
        {
            tree->entry_size *= 2;
            tree->entries = (index_t *) realloc(tree->entries, tree->entry_size * sizeof(index_t));
        }
    }
}

levtree_tree* levtree_tree_init(const char ** words, index_t words_count)
{
    levtree_tree *tree = malloc(sizeof(levtree_tree));
    tree->node_count = 0;
    tree->node_size = words_count * 2;
    tree->nodes = malloc(sizeof(levtree_levnode) * tree->node_size);
    tree->allocated = 0;
    tree->torealloc = 0;
    tree->entry_count = 0;
    tree->maxsize = 0;
    tree->checker = case_insensitive_checker;
    levtree_tree_set_algorithm(tree, LEVENSHTEIN);
    tree->entry_size = words_count;
    tree->entries = (index_t *) malloc(sizeof(index_t *)*words_count);
    tree->standing = (levtree_standing *) malloc(sizeof(levtree_standing));
    levtree_levnode_init(tree->nodes, '\0', 0);
    tree->node_count++;
    index_t i;

    for(i = 0; i < words_count; i++)
    {
        levtree_tree_add_word(tree, words[i], i);
    }

    //levtree_standing_init(tree->standing, standing_size);
    return tree;
}

void levtree_tree_free(levtree_tree * tree)
{
    levtree_tree_delete_rows(tree);
    free(tree->entries);
    free(tree->nodes);

    if(tree->allocated)
    {
        levtree_standing_free(tree->standing);
    }

    free(tree->standing);
    free(tree);
}



void lprint(levtree_standing * s)
{
    printf("----------STANDING----------\n");
    index_t i;
    levtree_result * r = s->bottom;

    for(i = 0; i < s->count; i++)
    {
        printf("node: %d, distance: %d, next: %p\n", r->id, r->distance, (void *)r->next);
        r = r->next;
    }
}

void levtree_tree_search(levtree_tree * tree, const char * wordkey, index_t n_of_matches)
{
    if(!tree->allocated)
    {
        levtree_tree_alloc_rows(tree, tree->maxsize);
        tree->allocated = 1;
    }
    else
    {
        levtree_standing_free(tree->standing);
    }

    if(tree->torealloc)
    {
        levtree_tree_realloc_rows(tree, tree->maxsize);
        tree->torealloc = 0;
    }

    levtree_standing_init(tree->standing, n_of_matches);
    index_t i, j, pathindex;
    index_t size = strlen((const char*)wordkey) + 1;
    uint32_t * keys = malloc(size * sizeof(uint32_t));
    size=0;
    while(1)
    {
        uint8_t charsz;
        uint32_t key = read_utf8_char(wordkey, &charsz);
        wordkey += charsz;
        keys[size++] = key;
        if(!key) break;
    }
    index_t * path = (index_t *) malloc(sizeof(index_t) * (tree->maxsize + 2));

    if(size > tree->maxsize)
    {
        levtree_tree_realloc_rows(tree, size);
        tree->maxsize = size;
    }

    tree->nodes[0].processed = 1;

    for(i = 0; i < size; i++)
    {
        tree->nodes[0].row[i] = i;
    }

    index_t ptr = 0, ref;

    for(i = 0; i < tree->entry_count; i++)
    {
        ref = tree->entries[i];
        ptr = ref;
        pathindex = 0;

        while(ptr)
        {
            path[pathindex++] = ptr;
            ptr = tree->nodes[ptr].parent;
        }

        path[pathindex++] = 0;
        ptr = ref;
        tree->nodes[ref].processed = 1;
        j = pathindex;
#ifdef DEBUG

        if(j > jmax)
        {
            jmax = j;
            printf("jmax: %u\n", jmax);
        }

#endif

        while(j-- > 0)
        {
            if(tree->nodes[path[j]].processed)
            {
                continue;
            }
            tree->distance_calculator(tree, keys, size, path, pathindex, j);
            tree->nodes[path[j]].processed = 1;
        }
        levtree_standing_add_result(tree->standing, tree->nodes[ref].id, tree->nodes[path[1]].row[size - 1]);
    }

    for(i = 0; i < tree->entry_count; i++)
    {
        ref = tree->entries[i];
        ptr = ref;

        while(tree->nodes[ptr].processed == 1)
        {
            tree->nodes[ptr].processed = 0;

            if(ptr != 0)
                ptr = tree->nodes[ptr].parent;
            else
                break;
        }
    }
    free(path);
    free(keys);
}


void levtree_tree_set_case_sensitive(levtree_tree * tree, uint8_t boolean)
{
    if(boolean)
    {
        tree->checker = case_sensitive_checker;
    }
    else
    {
        tree->checker = case_insensitive_checker;
    }
}

static void levenshtein_distance(levtree_tree * tree, const uint32_t * wordkey, index_t wordlen, index_t * path, index_t pathLength, index_t j)
{
    (void)(pathLength);
    index_t * prow = tree->nodes[tree->nodes[path[j]].parent].row;
    index_t * crow = tree->nodes[path[j]].row;
    crow[0] = prow[0] + 1;
    index_t k;

    for(k = 1; k < wordlen; k++)
    {
        if(tree->checker(tree->nodes[path[j]].key, wordkey[k - 1]))
        {
            crow[k] = prow[k - 1];
        }
        else
        {
            crow[k] = min3(crow[k - 1] + 1, prow[k] + 1, prow[k - 1] + 1);
        }
    }
}

static void damerau_levenshtein_distance(levtree_tree * tree, const uint32_t * wordkey, index_t wordlen, index_t * path, index_t pathLength, index_t j)
{
    index_t * prow = tree->nodes[tree->nodes[path[j]].parent].row;
    index_t * crow = tree->nodes[path[j]].row;
    index_t * pprow = tree->nodes[tree->nodes[path[j + 1]].parent].row;
    crow[0] = prow[0] + 1;
    index_t k;

    for(k = 1; k < wordlen; k++)
    {
        if(tree->checker(tree->nodes[path[j]].key, wordkey[k - 1]))
        {
            crow[k] = prow[k - 1];
        }
        else
        {
            crow[k] = min3(crow[k - 1] + 1, prow[k] + 1, prow[k - 1] + 1);
        }

        if(j < pathLength - 2 && k > 1 &&
           tree->checker(tree->nodes[path[j + 1]].key, wordkey[k - 1]) &&
           tree->checker(tree->nodes[path[j]].key, wordkey[k - 2]) &&
           !tree->checker(wordkey[k - 2], wordkey[k - 1])
          )
        {
            crow[k] = min(crow[k], pprow[k - 2] + 1);
        }
    }
}

void levtree_tree_set_algorithm(levtree_tree * tree, levtree_algorithm algo)
{
    switch(algo)
    {
    case LEVENSHTEIN:
        tree->distance_calculator = levenshtein_distance;
        break;

    case DAMERAU_LEVENSHTEIN:
        tree->distance_calculator = damerau_levenshtein_distance;
        break;
    }
}
