#include "Levtree.hpp"

#include <cstdlib>
#include <memory>

using namespace std;
using namespace levtree;

static inline wstring convert2wchar(const string& str)
{
    size_t mbslen = std::mbstowcs(NULL, &str[0], 0);
    wstring buf(mbslen + 1, '\0');
    std::mbstowcs(&buf[0], &str[0], mbslen);
    return buf;
}

LevtreeResult::LevtreeResult(size_t id, size_t distance, string word) :
        word(word), id(id), distance(distance)
{
}

Levtree::Levtree(const std::vector<string> &wordvector) :
        wordvector(wordvector)
{
    const size_t word_count = wordvector.size();
	//printf("SIZE: %u\n", size);
    std::vector<const uint8_t*> ptvector(word_count);
    for (size_t i = 0; i < word_count; i++)
	{
        ptvector[i] = (const uint8_t*) &wordvector[i][0];
	}
    tree = levtree_tree_init(ptvector.data(), word_count);
}

Levtree::~Levtree()
{
    levtree_tree_free(tree);
}

vector<LevtreeResult> Levtree::search(const string &text, size_t n)
{
    levtree_tree_search(tree, (const uint8_t *) text.data(), n);
	vector<LevtreeResult> results;
	levtree_result tmp;
    for (size_t i = 0; i < n; i++)
	{
        tmp = levtree_tree_get_result(tree, i);
		results.push_back(
				LevtreeResult(tmp.id, tmp.distance, wordvector[tmp.id]));
	}
	return results;
}

void Levtree::addWord(const string &word)
{
    levtree_tree_add_word(tree, (uint8_t *) word.data(), tree->entry_count);
    wordvector.push_back(word);
}

void Levtree::setAlgorithm(levtree_algorithm algo)
{
    levtree_tree_set_algorithm(tree, algo);
}

void Levtree::setCaseSensitive(bool b)
{
    levtree_tree_set_case_sensitive(tree, b ? 1 : 0);
}

string Levtree::getWord(size_t id)
{
    return wordvector[id].data();
}
