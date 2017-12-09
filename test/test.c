#include <stdio.h>
#include "levtree/levtree.h"
#include <locale.h>

int main()
{
    FILE * file = fopen(DICTIONARY_FILE, "r");
    const uint8_t * searches[] = {"camel", "coriolis", "mattel", "cruzer", "cpoper", "roublesoot"};

    const uint8_t * wordlist[] = {"csoa"};
    levtree_tree * tree = levtree_tree_init(wordlist, 1);
    index_t i, j;
    levtree_result res;
    const uint8_t buffer[50];
    i = 0;

    while(!feof(file))
    {
        fscanf(file, "%s\n", (char *) buffer);
        levtree_tree_add_word(tree, buffer, i++);
    }

    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 6; j++)
        {
            levtree_tree_search(tree, searches[j], 6);
        }
    }

    for(j = 0; j < 6; j++)
    {
        levtree_tree_search(tree, searches[j], 6);

        for(i = 0; i < tree->standing->count; i++)
        {
            res = levtree_tree_get_result(tree, i);
            printf("id: %u\tdistance: %u\n", res.id, res.distance);
        }
        puts("");
    }

    levtree_tree_free(tree);
    return 0;
}
