#include <stdio.h>
#include <string.h>

#include "levtree/levtree.h"
#include "dictionary.h"

int main()
{
    const char * searches[] = {"camel", "coriolis", "mattel", "cruzer", "cpoper", "roublesoot"};
    const char * wordlist[] = {"csoa"};
    levtree_tree * tree = levtree_tree_init(wordlist, 1);
    index_t i, j;
    levtree_result res;
    char buffer[1024];
    i = 0;
    size_t cursor = 0, start = 0;
    int c;
    while(cursor < sizeof(levtree_test_dictionary_file))
    {
        c = levtree_test_dictionary_file[cursor];
        if(c == '\n')
        {
            size_t sz = cursor - start;
            if(sz > 1024)
            {
                fprintf(stderr, "word too big, size: %zu\n", sz);
                exit(-1);
            }
            else if(sz > 1)
            {
                memcpy(buffer, &levtree_test_dictionary_file[start], sz);
                buffer[sz] = '\0';
                levtree_tree_add_word(tree, buffer, i++);
            }
            start = cursor+1;
        }
        cursor++;
    }

    for(i = 0; i < 30; i++)
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
