#include <stdio.h>
#include "levtree.h"
#include "wlevtree.h"
#include <locale.h>

int main2()
{
    setlocale(LC_ALL,"");
    wlevtree tree;
    wchar_t* wordlist[] = {L"città",L"ciao",L"coniglio",L"casa",L"cane",L"scuola",L"Papà", L"stacippa", L"Cornacchia"};
    wlevtree_init(&tree, wordlist,9);
    index_t i;
    levtree_result res;
    wlevtree_set_algorithm(&tree,DAMERAU_LEVENSHTEIN);
    wlevtree_search(&tree,L"apPà",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = wlevtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\t%ls\n",res.id,res.distance,wordlist[res.id]);
    }
    puts("");
    wlevtree_search(&tree,L"Corncachia",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = wlevtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\t%ls\n",res.id,res.distance,wordlist[res.id]);
    }
    wlevtree_add_word(&tree, L"scimmia", 9);
    wlevtree_search(&tree, L"schimma",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = wlevtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\t%ls\n",res.id,res.distance,wordlist[res.id]);
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
    levtree_search(&tree,"cosa",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = levtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    levtree_set_algorithm(&tree, DAMERAU_LEVENSHTEIN);
    levtree_search(&tree,"cosa",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = levtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }
    levtree_add_word(&tree, "scimmia", 7);
    levtree_search(&tree, "schimma",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = levtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }

    levtree_search(&tree, "schimma",4);
    for(i=0; i<tree.standing->count;i++)
    {
        res = levtree_get_result(&tree,i);
        printf("id: %u\tdistance: %u\n",res.id,res.distance);
    }

    levtree_free(&tree);
    return 0;
}
