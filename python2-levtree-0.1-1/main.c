#include <stdio.h>
#include "levtree.h"

int main()
{
    levtree tree;
    char* wordlist[] = {"ciao","cogliona","casa","cane", "scuola"};
    levtree_init(&tree, wordlist,5);
    index_t i;
    levtree_result res;
    levtree_search(&tree,"cosa",2);
    for(i=0; i<tree.standing->count;i++)
    {
        res = levtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    levtree_search(&tree,"cibo",2);
    for(i=0; i<tree.standing->count;i++)
    {
        res = levtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    levtree_free(&tree);
    return 0;
}
