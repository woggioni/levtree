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
	FILE *file = fopen("/usr/share/dict/cracklib-small","r");
	const char* searches[] = {"camel", "coriolis", "mattel", "cruzer", "cpoper", "roublesoot"};
    levtree tree;
    char* wordlist[] = {"csoa"};
    levtree_init(&tree, wordlist,1);
    index_t i,j;
    levtree_result res;
    char buffer[50];
    
    i=0;
    while(!feof(file))
    {
		fscanf(file, "%s\n", buffer);
		levtree_add_word(&tree, buffer, i++);
	}

	for(i=0; i<50; i++)
	{
		for(j=0; j<6; j++)
		{
			levtree_search(&tree, searches[j], 6);
		}
	}
	
	for(j=0; j<6; j++)
	{
		levtree_search(&tree, searches[j], 6);
		
		for(i=0; i<tree.standing->count;i++)
		{
			res = levtree_get_result(&tree,i);
			printf("id: %u\tdistance: %u\n",res.id,res.distance);
		}
		puts("");
	}

    levtree_free(&tree);
    return 0;
}
