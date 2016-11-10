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
    vector<wstring> wsvector(word_count);
    std::vector<wchar_t*> ptvector(word_count);
    for (size_t i = 0; i < word_count; i++)
	{
        const std::string& str = wordvector[i];
        wsvector[i] = convert2wchar(str);
        //const void *p1 = &wsvector[i][0], *p2 = &str[0];
        ptvector[i] = &wsvector[i][0];
		//printf("WORD: %ls\n", wordvector[i].c_str());
	}
    tree = wlevtree_init(ptvector.data(), word_count);
}

Levtree::~Levtree()
{
	wlevtree_free(tree);
}

vector<LevtreeResult> Levtree::search(const string &text, size_t n)
{
    wlevtree_search(tree, convert2wchar(text).data(), n);
	vector<LevtreeResult> results;
	levtree_result tmp;
    for (size_t i = 0; i < n; i++)
	{
		tmp = wlevtree_get_result(tree, i);
		results.push_back(
				LevtreeResult(tmp.id, tmp.distance, wordvector[tmp.id]));
	}
	return results;
}

void Levtree::addWord(const string &word)
{
    const auto& wstr = convert2wchar(word);
    wlevtree_add_word(tree, wstr.data(), tree->entry_count);
    wordvector.push_back(word);
}

void Levtree::setAlgorithm(levtree_algorithm algo)
{
	wlevtree_set_algorithm(tree, algo);
}

void Levtree::setCaseSensitive(bool b)
{
	wlevtree_set_case_sensitive(tree, b ? 1 : 0);
}

string Levtree::getWord(size_t id)
{
    return wordvector[id].data();
}
