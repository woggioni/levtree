#include <stdio.h>
#include "levtree.h"



/*
int main2(void)
{
    srand(time(NULL));
    index_t i;
    levtree_standing st;
    levtree_standing_init(&st,5);
    levtree_result res;
    res.next=NULL;
    for(i=0;i<10;i++)
    {
        res.distance = rand()%1000;
        res.id = rand()%100;
        printf("res: {node: %d, distance: %d}\n",res.id,res.distance);
        levtree_standing_add_result(&st,res.id,res.distance);
        lprint(&st);
    }
    levtree_standing_free(&st);
    return 0;
}*/

int main()
{
    levtree tree;
    const char* wordlist[] = {"ciao","cogliona","casa","cane", "scuola"};
    levtree_init(&tree, wordlist,5,2);
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
