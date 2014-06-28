#include <stdio.h>
#include "levtree.h"
#include "wlevtree.h"
#include <locale.h>

int main()
{
    setlocale(LC_ALL,"");
    wlevtree tree;
    wchar_t* wordlist[] = {L"città",L"ciao",L"coniglio",L"casa",L"cane",L"scuola",L"Papà", L"stacippa", L"Cornacchia"};
    wlevtree_init(&tree, wordlist,9);
    index_t i;
    levtree_result res;
    wtree_search_dl(&tree,L"apPà",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = wlevtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    puts("");
    wtree_search_dl(&tree,L"Corncachia",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = wlevtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    wlevtree_add_word(&tree, L"scimmia", 9);
    wtree_search_dl(&tree, L"schimma",4);
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
