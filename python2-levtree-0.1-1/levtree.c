#include <string.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include "levtree.h"

#define min(x,y) ((x) < (y) ? (x) : (y))
#define min3(a,b,c) ((a)< (b) ? min((a),(c)) : min((b),(c)))
#define min4(a,b,c,d) ((a)< (b) ? min3((a),(c),(d)) : min3((b),(c),(d)))

inline void levtree_alloc_rows(levtree* tree, index_t newsize)
{
    index_t i;
    for(i=0; i<tree->node_count; i++)
    {
        tree->nodes[i].row = (index_t*) malloc(newsize*sizeof(index_t));
    }
}

inline void levtree_realloc_rows(levtree* tree, index_t newsize)
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

inline void levtree_delete_rows(levtree* tree)
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

levtree_result levtree_get_result(levtree* tree, index_t pos)
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

inline void levtree_add_node(levtree *tree, char key, index_t index, index_t parent, index_t prev)
{
    tree->node_count++;
    if(tree->node_count >= tree->node_size)
    {
        tree->node_size *= 2;
        tree->nodes = (levnode*) realloc(tree->nodes, tree->node_size*sizeof(levnode));
    }
    levnode* node = &tree->nodes[tree->node_count-1];
    if(key)
    {
        levnode_init(node,key,0);
    }
    else
    {
        levnode_init(node,key,index);
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


void levtree_add_word(levtree* tree, const char* keyword, index_t id)
{
    index_t size;
    index_t initial_nodes=tree->node_count;
    index_t ki=0;
    index_t tnode=0,cnode,nnode;
    size=strlen(keyword)+1;
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
            if(tree->case_sensitive ? (tree->nodes[cnode].key == keyword[ki]) : (tolower(tree->nodes[cnode].key) == tolower(keyword[ki])) )
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
                    if(tree->case_sensitive ? (tree->nodes[nnode].key==keyword[ki]) : tolower(tree->nodes[nnode].key)==tolower(keyword[ki]) )
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
        levtree_add_node(tree,keyword[ki],id,tnode,cnode);
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

void levtree_init(levtree *tree, char **words, index_t words_count)
{
    tree->node_count = 0;
    tree->node_size = words_count*2;
    tree->nodes = malloc(sizeof(levnode)*tree->node_size);
    tree->allocated = 0;
    tree->torealloc = 0;
    tree->entry_count = 0;
    tree->maxsize = 0;
    tree->case_sensitive = 0;
    tree->entry_size = words_count;
    tree->entries = (index_t*) malloc(sizeof(index_t*)*words_count);
    tree->standing = (levtree_standing*) malloc(sizeof(levtree_standing));
    levnode_init(tree->nodes,'\0',0);
    tree->node_count++;
    index_t i;
    for(i=0; i<words_count; i++)
    {
        levtree_add_word(tree, words[i], i);
    }
    //levtree_standing_init(tree->standing, standing_size);
}

void levtree_free(levtree *tree)
{
    levtree_delete_rows(tree);
    free(tree->entries);
    free(tree->nodes);
    if(tree->allocated)
    {
        levtree_standing_free(tree->standing);
    }
    free(tree->standing);
}



void lprint(levtree_standing* s)
{
    printf("----------STANDING----------\n");
    index_t i;
    levtree_result *r = s->bottom.next;
    for(i=0; i<s->count; i++)
    {
        printf("node: %d, distance: %d, next: %p\n",r->id, r->distance, (void*)r->next);
        r = r->next;
    }
}

void tree_isearch(levtree* tree, const char* wordkey, index_t n_of_matches)
{

    if(!tree->allocated)
    {
        levtree_alloc_rows(tree,tree->maxsize);
        tree->allocated=1;
    }
    else
    {
        levtree_standing_free(tree->standing);
    }
    if(tree->torealloc)
    {
        levtree_realloc_rows(tree,tree->maxsize);
        tree->torealloc=0;
    }
    levtree_standing_init(tree->standing, n_of_matches);
    index_t i,j,k,pathindex;
    index_t size;
    size=strlen(wordkey)+1;
    index_t *path= (index_t*) malloc(sizeof(index_t)*(tree->maxsize+2));
    if(size>tree->maxsize)
    {
        levtree_realloc_rows(tree,size);
        tree->maxsize=size;
    }
    tree->nodes[0].processed=1;
    for(i=0; i<size;i++)
    {
        tree->nodes[0].row[i]=i;
    }

    index_t *crow,*prow;
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
            prow=tree->nodes[tree->nodes[path[j]].parent].row;
            crow=tree->nodes[path[j]].row;
            crow[0]=prow[0]+1;
            for(k=1;k<size;k++)
            {
                if(tolower(tree->nodes[path[j]].key) == tolower(wordkey[k-1]))
                {
                    crow[k]=prow[k-1];
                }
                else
                {
                    crow[k]=min3(crow[k-1]+1,prow[k]+1,prow[k-1]+1);
                }
            }
            tree->nodes[path[j]].processed=1;
        }
        levtree_standing_add_result(tree->standing,tree->nodes[ref].id,crow[size-1]);
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

void tree_search(levtree* tree, const char* wordkey, index_t n_of_matches)
{

    if(!tree->allocated)
    {
        levtree_alloc_rows(tree,tree->maxsize);
        tree->allocated=1;
    }
    else
    {
        levtree_standing_free(tree->standing);
    }
    if(tree->torealloc)
    {
        levtree_realloc_rows(tree,tree->maxsize);
        tree->torealloc=0;
    }
    levtree_standing_init(tree->standing, n_of_matches);
    index_t i,j,k,pathindex;
    index_t size;
    size=strlen(wordkey)+1;
    index_t *path= (index_t*) malloc(sizeof(index_t)*(tree->maxsize+2));
    if(size>tree->maxsize)
    {
        levtree_realloc_rows(tree,size);
        tree->maxsize=size;
    }
    tree->nodes[0].processed=1;
    for(i=0; i<size;i++)
    {
        tree->nodes[0].row[i]=i;
    }

    index_t *crow,*prow;
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
            prow=tree->nodes[tree->nodes[path[j]].parent].row;
            crow=tree->nodes[path[j]].row;
            crow[0]=prow[0]+1;
            for(k=1;k<size;k++)
            {
                if(tree->nodes[path[j]].key == wordkey[k-1])
                {
                    crow[k]=prow[k-1];
                }
                else
                {
                    crow[k]=min3(crow[k-1]+1,prow[k]+1,prow[k-1]+1);
                }
            }
            tree->nodes[path[j]].processed=1;
        }
        levtree_standing_add_result(tree->standing,tree->nodes[ref].id,crow[size-1]);
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

void tree_search_dl(levtree* tree, const char *wordkey, index_t n_of_matches)
{

    if(!tree->allocated)
    {
        levtree_alloc_rows(tree,tree->maxsize);
        tree->allocated=1;
    }
    else
    {
        levtree_standing_free(tree->standing);
    }
    if(tree->torealloc)
    {
        levtree_realloc_rows(tree,tree->maxsize);
        tree->torealloc=0;
    }
    levtree_standing_init(tree->standing, n_of_matches);
    index_t i,j,k,pathindex;
    index_t size;
    size=strlen(wordkey)+1;
    index_t *path= (index_t*) malloc(sizeof(index_t)*(tree->maxsize+2));
    if(size>tree->maxsize)
    {
        levtree_realloc_rows(tree,size);
        tree->maxsize=size;
    }
    tree->nodes[0].processed=1;
    for(i=0; i<size;i++)
    {
        tree->nodes[0].row[i]=i;
    }

    index_t *crow,*prow,*pprow;
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
            prow=tree->nodes[tree->nodes[path[j]].parent].row;
            crow=tree->nodes[path[j]].row;
            pprow=tree->nodes[tree->nodes[path[j+1]].parent].row;
            crow[0]=prow[0]+1;
            for(k=1;k<size;k++)
            {
                if(tree->nodes[path[j]].key==wordkey[k-1])
                {
                    crow[k]=prow[k-1];
                }
                else
                {
                    crow[k]=min3(crow[k-1]+1,prow[k]+1,prow[k-1]+1);
                }
                if(j<pathindex-2 && k>1 &&
                        tree->nodes[path[j+1]].key == wordkey[k-1] &&
                        tree->nodes[path[j]].key == wordkey[k-2] &&
                        wordkey[k-2] != wordkey[k-1]
                        )
                {
                    crow[k]=min(crow[k], pprow[k-2]+1);
                }
            }
            tree->nodes[path[j]].processed=1;
        }
        levtree_standing_add_result(tree->standing,tree->nodes[ref].id,crow[size-1]);
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

void tree_isearch_dl(levtree* tree, const char *wordkey, index_t n_of_matches)
{

    if(!tree->allocated)
    {
        levtree_alloc_rows(tree,tree->maxsize);
        tree->allocated=1;
    }
    else
    {
        levtree_standing_free(tree->standing);
    }
    if(tree->torealloc)
    {
        levtree_realloc_rows(tree,tree->maxsize);
        tree->torealloc=0;
    }
    levtree_standing_init(tree->standing, n_of_matches);
    index_t i,j,k,pathindex;
    index_t size;
    size=strlen(wordkey)+1;
    index_t *path= (index_t*) malloc(sizeof(index_t)*(tree->maxsize+2));
    if(size>tree->maxsize)
    {
        levtree_realloc_rows(tree,size);
        tree->maxsize=size;
    }
    tree->nodes[0].processed=1;
    for(i=0; i<size;i++)
    {
        tree->nodes[0].row[i]=i;
    }

    index_t *crow,*prow,*pprow;
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
            prow=tree->nodes[tree->nodes[path[j]].parent].row;
            crow=tree->nodes[path[j]].row;
            pprow=tree->nodes[tree->nodes[path[j+1]].parent].row;
            crow[0]=prow[0]+1;
            for(k=1;k<size;k++)
            {
                if(tolower(tree->nodes[path[j]].key)==tolower(wordkey[k-1]))
                {
                    crow[k]=prow[k-1];
                }
                else
                {
                    crow[k]=min3(crow[k-1]+1,prow[k]+1,prow[k-1]+1);
                }
                if(j<pathindex-2 && k>1 &&
                        tolower(tree->nodes[path[j+1]].key) == tolower(wordkey[k-1]) &&
                        tolower(tree->nodes[path[j]].key) == tolower(wordkey[k-2]) &&
                        tolower(wordkey[k-2]) != tolower(wordkey[k-1])
                        )
                {
                    crow[k]=min(crow[k], pprow[k-2]+1);
                }
            }
            tree->nodes[path[j]].processed=1;
        }
        levtree_standing_add_result(tree->standing,tree->nodes[ref].id,crow[size-1]);
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
