#ifndef LEVTREE_HPP
#define LEVTREE_HPP


//#include "../wlevtree.h"

#include <string>
#include <vector>

#include "../common.h"
#include "../wlevnode.h"
#include "../levtree_standing.h"

typedef struct wlevtree wlevtree;

struct wlevtree
{
    index_t maxsize;
    byte_t allocated;
    byte_t torealloc;
    wlevnode* nodes;
    index_t node_count;
    index_t node_size;
    index_t* entries; //vettore di interi che contiene la posizione di ogni parola nel vettore dei nodi
    index_t entry_count;
    index_t entry_size;
    levtree_standing* standing;
    byte_t (*checker)(wchar_t key1, wchar_t key2);
    void (*distance_calculator)(wlevtree* tree, const wchar_t* wordkey, index_t wordlen, index_t* path, index_t pathLength, index_t j);
};

extern "C"
{
void wlevtree_init(wlevtree* tree, wchar_t **words, index_t words_count);
void wlevtree_free(wlevtree* tree);
void wlevtree_search(wlevtree* tree, const wchar_t* wordkey,
		index_t n_of_matches);
void wlevtree_add_word(wlevtree* tree, const wchar_t *keyword, index_t id);
levtree_result wlevtree_get_result(wlevtree* tree, index_t index);
void wlevtree_set_algorithm(wlevtree *tree, levtree_algorithm algo);
void wlevtree_set_case_sensitive(wlevtree *tree, byte_t boolean);

}


namespace levtree
{

class LevtreeResult
{
public:
	LevtreeResult(index_t id, index_t distance, std::wstring word);

	std::wstring word;
	index_t id;
	index_t distance;

};

class Levtree
{
	wlevtree* tree;
	std::vector<std::wstring> wordvector;
public:
	void addWord(std::wstring &word);
	std::wstring getWord(index_t id);
	Levtree(std::vector<std::wstring> &wordvector);
	~Levtree();
	std::vector<LevtreeResult> search(std::wstring &text,
			index_t number_of_matches = 4);
	void setCaseSensitive(bool b);
	void setAlgorithm(levtree_algorithm algo);
};

}
#endif // LEVTREE_HPP
