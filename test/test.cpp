#include <cstdio>
#include <cstring>
#include <string>
#include <codecvt>
#include <chrono>
#include <map>

#include "levtree++/Levtree.hpp"
#include "dictionary.h"

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
    vector<string> wordlist;
    size_t cursor = 0, start = 0;
    int c;
    while(cursor < sizeof(levtree_test_dictionary_file))
    {
        c = levtree_test_dictionary_file[cursor];
        if(c == '\n')
        {
            size_t sz = cursor - start;
            if(sz > 1)
            {
                wordlist.push_back(std::string((const char *) &levtree_test_dictionary_file[start], sz));
            }
            start = cursor+1;
        }
        cursor++;
    }

    string searches[] = {"camel", "coriolis", "mattel", "cruzer", "cpoper", "roublesoot"};
    index_t i, j;
    Levtree tree(wordlist);
    Chronometer chr;

    for(i = 0; i < 30; i++)
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

