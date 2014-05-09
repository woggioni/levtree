#ifndef LEVTREE_STANDING_H
#define LEVTREE_STANDING_H

#include "levnode.h"

typedef struct levtree_result levtree_result;

struct levtree_result
{
    index_t id;
    index_t distance;
    levtree_result* next;
};

typedef struct
{
    levtree_result* entries;
    levtree_result bottom;
    index_t size;
    index_t count;
}levtree_standing;


inline void levtree_standing_init(levtree_standing *s, index_t size);

inline void levtree_standing_free(levtree_standing *s);

inline void levtree_standing_insert(levtree_standing *s, index_t pos, levtree_result res);

inline void levtree_standing_add_result(levtree_standing* s, index_t id, index_t dist);


#endif // LEVTREE_STANDING_H
