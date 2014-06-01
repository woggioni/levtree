#include <stdio.h>
#include "levtree.h"
#include "wlevtree.h"
#include <locale.h>

int main2()
{
    setlocale(LC_ALL,"");
    wlevtree tree;
    wchar_t* wordlist[] = {L"Papà",L"Pupa",L"pipà",L"Pappa",L"cornacciha",L"cane",L"scuola"};
    wlevtree_init(&tree, wordlist,7);
    index_t i;
    levtree_result res;
    wtree_isearch_dl(&tree,L"apPà",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = wlevtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    puts("");
    wtree_isearch_dl(&tree,L"carnocchia",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = wlevtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    wlevtree_free(&tree);
    return 0;
}

int main()
{
    levtree tree;
    char* wordlist[] = {"csoa","ciao","ocsa","coniglio","casa","cane", "scuola"};
    levtree_init(&tree, wordlist,7);
    index_t i;
    levtree_result res;
    tree_search_dl(&tree,"cosa",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = levtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    tree_isearch_dl(&tree,"cosa",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = levtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    levtree_free(&tree);
    return 0;
}
