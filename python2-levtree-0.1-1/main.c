#include <stdio.h>
#include "levtree.h"
#include "wlevtree.h"
#include <locale.h>

int main()
{
    setlocale(LC_ALL,"");
    wlevtree tree;
    wchar_t* wordlist[] = {L"ciao",L"coniglio",L"casa",L"cane",L"scuola"};
    wlevtree_init(&tree, wordlist,5);
    index_t i;
    levtree_result res;
    wlevtree_search(&tree,L"cosa",2);
    for(i=0; i<tree.standing->count;i++)
    {
        res = wlevtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    wlevtree_search(&tree,L"cibo",2);
    for(i=0; i<tree.standing->count;i++)
    {
        res = wlevtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    wlevtree_free(&tree);
    return 0;
}

int main2()
{
    levtree tree;
    char* wordlist[] = {"ciao","coniglio","casa","cane", "scuola"};
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
