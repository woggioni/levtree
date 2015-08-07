#include "Levtree.hpp"

#include <cstdlib>

using namespace std;
using namespace levtree;

LevtreeResult::LevtreeResult(index_t id, index_t distance, wstring word) :
		id(id), distance(distance), word(word)
{
	;
}

Levtree::Levtree(vector<wstring> &wordvector) :
		wordvector(wordvector)
{
	tree = (wlevtree*) malloc(sizeof(wlevtree));
	const index_t size = wordvector.size();
	//printf("SIZE: %u\n", size);
	wchar_t *ptvector[size];
	for (index_t i = 0; i < size; i++)
	{
		ptvector[i] = (wchar_t*) wordvector[i].c_str();
		//printf("WORD: %ls\n", wordvector[i].c_str());
	}


	wlevtree_init(tree, ptvector, size);
}

Levtree::~Levtree()
{
	wlevtree_free(tree);
}

vector<LevtreeResult> Levtree::search(wstring &text, index_t n)
{
	wlevtree_search(tree, text.c_str(), n);
	vector<LevtreeResult> results;
	levtree_result tmp;
	for (index_t i = 0; i < n; i++)
	{
		tmp = wlevtree_get_result(tree, i);
		results.push_back(
				LevtreeResult(tmp.id, tmp.distance, wordvector[tmp.id]));
	}
	return results;
}

void Levtree::addWord(wstring &word)
{
	wlevtree_add_word(tree, word.c_str(), tree->entry_count);
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

wstring Levtree::getWord(index_t id)
{
	return wordvector[id];
}

