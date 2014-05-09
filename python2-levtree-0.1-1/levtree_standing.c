#include <stdlib.h>
#include "levtree_standing.h"

extern void levtree_standing_free(levtree_standing *s)
{
    free(s->entries);
}

extern void levtree_standing_init(levtree_standing *s, index_t size)
{
    s->entries = (levtree_result*) malloc(sizeof(levtree_result)*size);
    s->size = size;
    s->bottom.next = s->entries;
    s->bottom.distance = -1;
    s->count = 0;
}


extern void levtree_standing_insert(levtree_standing *s, index_t pos, levtree_result res)
{
    index_t i=0;
    levtree_result *tres;
    tres = &s->bottom;
    levtree_result *tmp;
    if(s->count < s->size)
    {
        tmp = &s->entries[s->count];
    }
    else
    {
        tmp = s->bottom.next;
        s->bottom.next = tmp->next;
        if(pos)
            pos--;
    }
    *tmp = res;
    while(tres)
    {
        if(i==pos)
        {
            tmp->next = tres->next;
            tres->next = tmp;
            if(s->count<s->size)
            {
                s->count++;
            }
            break;
        }
        tres = tres->next;
        i++;
    }
}

extern void levtree_standing_add_result(levtree_standing* s, index_t id, index_t dist)
{
    levtree_result entry = {id,dist,NULL};
    index_t i;
    if(s->count == 0)
    {
        for(i=0; i<s->size; i++)
        {
            s->entries[i] = entry;
            if(i<s->size-1)
            {
                s->entries[i].next = &s->entries[i+1];
            }
        }
        s->count++;
        return;
    }
    levtree_result *r = &s->bottom;
    index_t insertion_point;
    byte_t insert=0;
    if(s->count < s->size)
    {
        insert=1;
        insertion_point = 0;
    }
    for(i=0; i<=s->count; i++)
    {
        if(dist < r->distance)
        {
            insert=1;
            insertion_point = i;
        }
        r = r->next;
    }
    if(insert)
    {
        levtree_standing_insert(s,insertion_point,entry);
    }
}
