#include <stdlib.h>
#include <stdio.h>
#include "levtree_standing.h"

static inline void levtree_standing_print(levtree_standing *s)
{
	levtree_result *r = s->bottom;
	while(r)
	{
		printf("{id: %d, distance: %d, next: %p}\n", r->id, r->distance, r->next);
		r = r->next;
	}
}

extern void levtree_standing_free(levtree_standing *s)
{
    free(s->entries);
}

extern void levtree_standing_init(levtree_standing *s, index_t size)
{
    s->entries = (levtree_result*) malloc(sizeof(levtree_result)*size);
    s->size = size;
    s->bottom = s->entries;
    s->count = 0;
}


extern void levtree_standing_insert(levtree_standing *s, index_t pos, levtree_result res)
{
    res.next = NULL;
           
    levtree_result *cur;
    levtree_result *new_result, *old;
    if(s->count < s->size)
    {
        new_result = &s->entries[s->count];
        s->count++;
    }
    else
    {
        new_result = s->bottom;
        s->bottom = new_result->next;
        if(pos) pos--;
    }
    *new_result = res;
    if(pos == 0)
    {
		old = s->bottom;
		s->bottom = new_result;
		new_result->next = old;
		return;
	}
	
	index_t i=0;
	cur = s->bottom;
    while(cur)
    {
        if(++i==pos)
        {
			old = cur->next;
			cur->next = new_result;
			
			new_result->next = old;
			if(s->bottom == new_result)
			{
				s->bottom = new_result;
			}
            break;
        }
        cur = cur->next;
    }
}

extern void levtree_standing_add_result(levtree_standing* s, index_t id, index_t dist)
{	
    levtree_result entry = {id,dist,NULL};
    
    index_t i;
    if(s->count == 0)
    {
		s->entries[0] = entry;
        s->count++;
        return;
    }
    levtree_result *r = s->bottom;
    index_t insertion_point;
    byte_t insert=0;
    
    if(s->count < s->size)
    {
        insert=1;
        insertion_point = 0;
    }
	for(i=1; i <= s->count; i++)
	{
		if(dist < r->distance)
		{
			insert=1;
			insertion_point = i;
		}
		r = r->next;
	}
	
	fflush(stdout);
    if(insert)
    {
        levtree_standing_insert(s,insertion_point,entry);
    }
}
