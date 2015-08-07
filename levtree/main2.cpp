#include <cstdio>
#include <cstring>
#include <sstream>
#include <fstream>
#include <codecvt>

#include "cpp/Levtree.hpp"


using namespace levtree;
using namespace std;


int main()
{

    std::wifstream wif("/usr/share/dict/cracklib-small");
    wstring word;
    vector<wstring> wordlist;

    while (std::getline(wif, word))
    {
    	wordlist.push_back(word);
    }

	wstring searches[] = {wstring(L"camel"), wstring(L"coriolis"), wstring(L"mattel"), wstring(L"cruzer"), wstring(L"cpoper"), wstring(L"roublesoot")};

	index_t i,j;

    Levtree tree(wordlist);

	for(i=0; i<50; i++)
	{
		for(j=0; j<6; j++)
		{
			tree.search(searches[j],6);
		}
	}

	
	for(j=0; j<6; j++)
	{
		vector<LevtreeResult> standing = tree.search(searches[j],6);
		
		for(LevtreeResult res : standing)
		{
			printf("id: %u\tdistance: %u\t word: %ls\n",res.id,res.distance, tree.getWord(res.id).c_str());
		}
		puts("");
	}

    return 0;
}

/*
int main()
{
	FILE *file = fopen("/usr/share/dict/cracklib-small","r");
	const wchar_t* searches[] = {L"camel", L"coriolis", L"mattel", L"cruzer", L"cpoper", L"roublesoot"};
    wlevtree tree;
    wchar_t* wordlist[] = {L"csoa"};
    wlevtree_init(&tree, wordlist,1);
    index_t i,j;
    levtree_result res;
    wchar_t buffer[50];

    i=0;
    while(!feof(file))
    {
		fscanf(file, "%s\n", buffer);
		wlevtree_add_word(&tree, buffer, i++);
	}

	for(i=0; i<50; i++)
	{
		for(j=0; j<6; j++)
		{
			wlevtree_search(&tree, searches[j], 6);
		}
	}

	for(j=0; j<6; j++)
	{
		wlevtree_search(&tree, searches[j], 6);

		for(i=0; i<tree.standing->count;i++)
		{
			res = wlevtree_get_result(&tree,i);
			printf("id: %u\tdistance: %u\n",res.id,res.distance);
		}
		puts("");
	}

    wlevtree_free(&tree);
    return 0;
}*/
