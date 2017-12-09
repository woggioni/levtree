#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
typedef enum
{
    LEVENSHTEIN,
    DAMERAU_LEVENSHTEIN
}
levtree_algorithm;

typedef uint32_t index_t;
#endif // COMMON_H
