#ifndef LEVTREE_HPP
#define LEVTREE_HPP


//#include "../wlevtree.h"

#include <string>
#include <vector>

extern "C"
{
#include "../wlevtree.h"
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
