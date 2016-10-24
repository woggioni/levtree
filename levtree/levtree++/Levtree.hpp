#ifndef LEVTREE_HPP
#define LEVTREE_HPP

#include <string>
#include <vector>

extern "C"
{
#include "levtree/wlevtree.h"
}

namespace levtree
{
    struct LevtreeResult
    {
        LevtreeResult() = default;
        LevtreeResult(size_t id, size_t distance, std::string word);

        std::string word;
        size_t id;
        size_t distance;

    };

    class Levtree
    {
        wlevtree* tree;
        std::vector<std::string> wordvector;
    public:
        void addWord(const std::string &word);
        std::string getWord(size_t id);
        Levtree(const std::vector<std::string> &wordvector = std::vector<std::string>());
        ~Levtree();
        std::vector<LevtreeResult> search(const std::string &text,
                size_t number_of_matches = 4);
        void setCaseSensitive(bool b);
        void setAlgorithm(levtree_algorithm algo);
    };
}

#endif // LEVTREE_HPP
