#include "cobjc.h"
#include <linux/hashtable.h>
#include <linux/mutex.h>

#ifndef _HCMAP_H
#define _HCMAP_H

#define MAP_CAPACITY 200

typedef struct _co_map
{
    cbase cbase;
    rwlock_t lock;
    uint64_t count;
    struct hlist_head table[MAP_CAPACITY];
}co_map,*co_map_t;

typedef void (*cmap_enum_func)(co_map_t map,void *key,void *value,bool *stop,bool *delete,void *udata);

extern cclass map_meta_class;

void* cmap_get(co_map_t map,uint64_t key);
bool cmap_put(co_map_t map,void* obj);
void cmap_remove(co_map_t map,uint64_t key);
void cmap_empty(co_map_t map);
uint64_t cmap_count(co_map_t map);
void cmap_enumeration(co_map_t map,cmap_enum_func cb,bool monopoly,void *udata);

#endif