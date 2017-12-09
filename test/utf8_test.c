#include <locale.h>
#include <levtree/levtree.h>
#include <stdio.h>

int main()
{
    setlocale(LC_ALL, "");
    levtree_tree * tree;
    const char * wordlist[] = {"città", "ciao", "coniglio", "casa", "cane", "scuola", "Papà", "stacippa", "Cornacchia"};
    tree = levtree_tree_init(wordlist, 9);
    index_t i;
    levtree_result res;
    levtree_tree_set_algorithm(tree, DAMERAU_LEVENSHTEIN);
    levtree_tree_search(tree, "apPà", 4);

    for(i = 0; i < tree->standing->count; i++)
    {
        res = levtree_tree_get_result(tree, i);
        printf("id: %u\tdistance: %u\t%s\n", res.id, res.distance, wordlist[res.id]);
    }

    puts("");
    levtree_tree_search(tree, "Corncachia", 4);

    for(i = 0; i < tree->standing->count; i++)
    {
        res = levtree_tree_get_result(tree, i);
        printf("id: %u\tdistance: %u\t%s\n", res.id, res.distance, wordlist[res.id]);
    }
    puts("");

    levtree_tree_add_word(tree, "scimmia", 9);
    levtree_tree_search(tree, "schimma", 4);

    for(i = 0; i < tree->standing->count; i++)
    {
        res = levtree_tree_get_result(tree, i);
        if(res.id < 9)
            printf("id: %u\tdistance: %u\t%s\n", res.id, res.distance, wordlist[res.id]);
        else
            printf("id: %u\tdistance: %u\t%s\n", res.id, res.distance, "scimmia");
    }

    levtree_tree_free(tree);
    return 0;
}
