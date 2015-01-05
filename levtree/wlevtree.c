#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <wctype.h>
#include "wlevtree.h"

#define min(x,y) ((x) < (y) ? (x) : (y))
#define min3(a,b,c) ((a)< (b) ? min((a),(c)) : min((b),(c)))
#define min4(a,b,c,d) ((a)< (b) ? min3((a),(c),(d)) : min3((b),(c),(d)))

extern inline void wlevtree_alloc_rows(wlevtree* tree, index_t newsize);
extern inline void wlevtree_realloc_rows(wlevtree* tree, index_t newsize);
extern inline void wlevtree_delete_rows(wlevtree* tree);


byte_t w_case_insensitive_checker(wchar_t k1, wchar_t k2)
{
    return towlower(k1)==towlower(k2);
}

byte_t w_case_sensitive_checker(wchar_t k1, wchar_t k2)
{
    return k1==k2;
}

inline void wlevtree_alloc_rows(wlevtree* tree, index_t newsize)
{
    index_t i;
    for(i=0; i<tree->node_count; i++)
    {
        tree->nodes[i].row = (index_t*) malloc(newsize*sizeof(index_t));
    }
}

inline void wlevtree_realloc_rows(wlevtree* tree, index_t newsize)
{
    index_t i;
    for(i=0; i<tree->node_count; i++)
    {
        if(tree->nodes[i].row)
        {
            tree->nodes[i].row = (index_t*) realloc(tree->nodes[i].row, newsize*sizeof(index_t));
        }
        else
        {
            tree->nodes[i].row = (index_t*) malloc(newsize*sizeof(index_t));
        }
    }
}

inline void wlevtree_delete_rows(wlevtree* tree)
{
    index_t i;
    for(i=0; i<tree->node_count; i++)
    {
        if(tree->nodes[i].row)
        {
            free(tree->nodes[i].row);
        }
    }
}

levtree_result wlevtree_get_result(wlevtree* tree, index_t pos)
{
    pos = tree->standing->count-pos-1;
    levtree_result* res = tree->standing->bottom.next;
    index_t i;
    for(i=0; i<tree->standing->count; i++)
    {
        if(i==pos)
        {
            break;
        }
        res=res->next;
    }
    return *res;
}

inline void wlevtree_add_node(wlevtree *tree, wchar_t key, index_t index, index_t parent, index_t prev)
{
    tree->node_count++;
    if(tree->node_count >= tree->node_size)
    {
        tree->node_size *= 2;
        tree->nodes = (wlevnode*) realloc(tree->nodes, tree->node_size*sizeof(wlevnode));
    }
    wlevnode* node = &tree->nodes[tree->node_count-1];
    if(key != 0)
    {
        wlevnode_init(node,key,0);
    }
    else
    {
        wlevnode_init(node,key,index);
    }
    node->parent = parent;
    node->prev = prev;
    if(!prev)
    {
        tree->nodes[parent].child = tree->node_count-1;
    }
    else
    {
        tree->nodes[prev].next = tree->node_count-1;
    }
}


void wlevtree_add_word(wlevtree* tree, const wchar_t* keyword, index_t id)
{
    if(tree->allocated)
    {
        wlevtree_delete_rows(tree);
        tree->allocated = 0;
    }
    index_t size;
    index_t initial_nodes=tree->node_count;
    index_t ki=0;
    index_t tnode=0,cnode,nnode;
    size = wcslen(keyword)+ 1;
    if(size>tree->maxsize)
    {
        tree->maxsize=size;
        if(tree->allocated)
        {
            tree->torealloc=1;
        }
    }
    start:
    while(ki<size)
    {
        nnode=0;
        cnode=tree->nodes[tnode].child;
        if(cnode)
        {
            if(tree->nodes[cnode].key == keyword[ki])
            {
                tnode=cnode;
                ki++;
                continue;
            }
            else
            {
                nnode = tree->nodes[cnode].next;
                while(nnode)
                {
                    if(tree->nodes[nnode].key==keyword[ki])
                    {
                        tnode=nnode;
                        ki++;
                        goto start;
                    }
                    cnode=nnode;
                    nnode=tree->nodes[nnode].next;
                }

            }
        }
        wlevtree_add_node(tree,keyword[ki],id,tnode,cnode);
        ki++;
        tnode=tree->node_count-1;
    }
    if(tree->node_count>initial_nodes)
    {
        tree->entry_count++;
        tree->entries[tree->entry_count-1] = tree->node_count-1;
        if(tree->entry_count >= tree->entry_size)
        {
            tree->entry_size *= 2;
            tree->entries = (index_t*) realloc(tree->entries, tree->entry_size*sizeof(index_t));
        }
    }
}

void wlevtree_init(wlevtree *tree, wchar_t **words, index_t words_count)
{
    tree->node_count = 0;
    tree->node_size = words_count*2;
    tree->nodes = malloc(sizeof(levnode)*tree->node_size);
    tree->allocated = 0;
    tree->torealloc = 0;
    tree->entry_count = 0;
    tree->maxsize = 0;
    tree->checker = w_case_insensitive_checker;
    tree->entry_size = words_count;
    tree->entries = (index_t*) malloc(sizeof(index_t*)*words_count);
    tree->standing = (levtree_standing*) malloc(sizeof(levtree_standing));
    wlevtree_set_algorithm(tree, LEVENSHTEIN);
    wlevnode_init(tree->nodes,L'\0',0);
    tree->node_count++;
    index_t i;
    for(i=0; i<words_count; i++)
    {
        wlevtree_add_word(tree, words[i], i);
    }
    //levtree_standing_init(tree->standing, standing_size);
}

void wlevtree_free(wlevtree *tree)
{
    wlevtree_delete_rows(tree);
    free(tree->entries);
    free(tree->nodes);
    if(tree->allocated)
    {
        levtree_standing_free(tree->standing);
    }
    free(tree->standing);
}

void wlevtree_search(wlevtree* __restrict__ tree, const wchar_t* __restrict__ wordkey, index_t n_of_matches)
{
    if(!tree->allocated)
    {
        wlevtree_alloc_rows(tree,tree->maxsize);
        tree->allocated=1;
    }
    else
    {
        levtree_standing_free(tree->standing);
    }
    if(tree->torealloc)
    {
        wlevtree_realloc_rows(tree,tree->maxsize);
        tree->torealloc=0;
    }
    levtree_standing_init(tree->standing, n_of_matches);
    index_t i,j,pathindex;
    index_t size;
    size=wcslen(wordkey)+1;
    index_t* __restrict__ path= (index_t*) malloc(sizeof(index_t)*(tree->maxsize+2));
    if(size>tree->maxsize)
    {
        wlevtree_realloc_rows(tree,size);
        tree->maxsize=size;
    }
    tree->nodes[0].processed=1;
    for(i=0; i<size;i++)
    {
        tree->nodes[0].row[i]=i;
    }

    index_t ptr=0,ref;

    for(i=0;i<tree->entry_count;i++)
    {
        ref=tree->entries[i];
        ptr=ref;
        pathindex=0;
        while(ptr)
        {
            path[pathindex++]=ptr;
            ptr=tree->nodes[ptr].parent;
        }
        path[pathindex++]=0;

        ptr=ref;
        tree->nodes[ref].processed=1;
        j=pathindex;
#ifdef DEBUG
        if(j>jmax)
        {
            jmax=j;
            printf("jmax: %u\n",jmax);
        }
#endif

        while(j-->0)
        {
            if(tree->nodes[path[j]].processed)
            {
                continue;
            }
            tree->distance_calculator(tree,wordkey,size,path,pathindex,j);
            tree->nodes[path[j]].processed=1;
        }
        levtree_standing_add_result(tree->standing,tree->nodes[ref].id,tree->nodes[path[1]].row[size-1]);
    }

    for(i=0;i<tree->entry_count;i++)
    {
        ref=tree->entries[i];
        ptr=ref;
        while(tree->nodes[ptr].processed == 1)
        {
            tree->nodes[ptr].processed=0;
            if(ptr!=0)
                ptr=tree->nodes[ptr].parent;
            else
                break;
        }
    }
    free(path);
}

inline void print_row(index_t *row, index_t len)
{
    index_t i;
    for(i=0; i<len; i++)
    {
        printf("%u\t", row[i]);
    }
    printf("\n");
}

inline void print_word(wchar_t* wordkey)
{
    index_t i,l;
    l=wcslen(wordkey);
    printf(" \t \t");

    for(i=0; i<l; i++)
    {
        printf("%lc\t", wordkey[i]);
    }
    printf("\n");
}

void wlevtree_set_case_sensitive(wlevtree *tree, byte_t boolean)
{
    if(boolean)
    {
        tree->checker = w_case_sensitive_checker;
    }
    else
    {
        tree->checker = w_case_insensitive_checker;
    }
}

void w_levenshtein_distance(wlevtree* __restrict__ tree, const wchar_t* __restrict__ wordkey, index_t wordlen, index_t* __restrict__ path, index_t pathLength, index_t j)
{
    index_t* __restrict__ prow = tree->nodes[tree->nodes[path[j]].parent].row;
    index_t* __restrict__ crow = tree->nodes[path[j]].row;
    crow[0]=prow[0]+1;
    index_t k;
    for(k=1; k<wordlen; k++)
    {
        if (tree->checker(tree->nodes[path[j]].key, wordkey[k - 1]))
        {
            crow[k] = prow[k - 1];
        } else
        {
            crow[k] = min3(crow[k - 1] + 1, prow[k] + 1, prow[k - 1] + 1);
        }
    }
}

void w_damerau_levenshtein_distance(wlevtree* __restrict__ tree, const wchar_t* __restrict__ wordkey, index_t wordlen, index_t* __restrict__ path, index_t pathLength, index_t j)
{
    index_t* __restrict__ prow = tree->nodes[tree->nodes[path[j]].parent].row;
    index_t* __restrict__ crow = tree->nodes[path[j]].row;
    index_t* __restrict__ pprow = tree->nodes[tree->nodes[path[j+1]].parent].row;
    crow[0]=prow[0]+1;
    index_t k;
    for(k=1; k<wordlen; k++)
    {
        if (tree->checker(tree->nodes[path[j]].key, wordkey[k - 1]))
        {
            crow[k] = prow[k - 1];
        } else
        {
            crow[k] = min3(crow[k - 1] + 1, prow[k] + 1, prow[k - 1] + 1);
        }
        if (j < pathLength - 2 && k > 1 &&
                tree->checker(tree->nodes[path[j+1]].key, wordkey[k - 1]) &&
                tree->checker(tree->nodes[path[j]].key, wordkey[k - 2]) &&
                !tree->checker(wordkey[k - 2], wordkey[k - 1])
                )
        {
            crow[k] = min(crow[k], pprow[k - 2] + 1);
        }
    }
}

void wlevtree_set_algorithm(wlevtree *tree, levtree_algorithm algo)
{
    switch(algo)
    {
    case LEVENSHTEIN:
        tree->distance_calculator = w_levenshtein_distance;
        break;
    case DAMERAU_LEVENSHTEIN:
        tree->distance_calculator = w_damerau_levenshtein_distance;
        break;
    }
}
