#include <cstdio>
#include <cstring>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <chrono>
#include <map>

#include "levtree++/Levtree.hpp"

#ifdef EMSCRIPTEN
//#include <emscripten/bind.h>
//EMSCRIPTEN_BINDINGS(stl_wrappers) {
//    emscripten::asm ()register_vector<std::string>("VectorString");
//}
#endif

using namespace levtree;
using namespace std;

class Chronometer
{
    std::chrono::high_resolution_clock::time_point start;
    std::map<std::string, std::chrono::high_resolution_clock::duration> accumulator;

public:
    Chronometer()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    void reset();

    template<typename T = std::chrono::milliseconds>
    long elapsed()
    {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<T>(now - start).count();
    }

    template<typename T = std::chrono::milliseconds>
    static long probeTime(const std::string & probeName)
    {
        return std::chrono::duration_cast<T>(getInstance().accumulator.at(probeName)).count();
    }

    class ChronometerProbe
    {
        std::string name;
        Chronometer & chr;
        bool stopped;
        std::chrono::high_resolution_clock::time_point start;
    public:
        ChronometerProbe(const std::string & name, Chronometer & chr);

        void stop();
        ~ChronometerProbe();
    };

    ChronometerProbe getProbe(const std::string & probeName);
    static Chronometer & getInstance();
};

int main()
{
    std::ifstream ifs(DICTIONARY_FILE);

    if(!ifs)
    {
        fprintf(stderr, "Cannot open input file '%s'", DICTIONARY_FILE);
        exit(-1);
    }

    string word;
    vector<string> wordlist;

    while(std::getline(ifs, word))
    {
        wordlist.push_back(word);
    }

    string searches[] = {"camel", "coriolis", "mattel", "cruzer", "cpoper", "roublesoot"};
    index_t i, j;
    Levtree tree(wordlist);
    Chronometer chr;

    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 6; j++)
        {
            tree.search(searches[j], 6);
        }
    }

    printf("Elapsed time: %.3f s\n", chr.elapsed() / 1000.0);

    for(j = 0; j < 6; j++)
    {
        vector<LevtreeResult> standing = tree.search(searches[j], 6);

        for(LevtreeResult res : standing)
        {
            printf("id: %zd\tdistance: %zd\t word: %s\n", res.id, res.distance, tree.getWord(res.id).c_str());
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
