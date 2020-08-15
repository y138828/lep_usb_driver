#include "cmap.h"

void co_map_dealloc(void *aself)
{
    co_map_t self = (co_map_t)aself;
    struct hlist_node *p = 0,*n = 0;
    int i = 0;
    for ( i = 0; i < MAP_CAPACITY; i++)
    {
        hlist_for_each_safe(p,n,&self->table[i])
        {
            cbase_t base = hlist_entry(p,cbase,node);
            hlist_del(&base->node);
            co_release(base);
        }
    }
    // rwlock_release(&self->lock);
}
void* co_map_init(void *aself)
{
    co_map_t map = (co_map_t)aself;
    rwlock_init(&map->lock);
    hash_init(map->table);
    map->count = 0;
    return aself;
}

cclass map_meta_class = {
    .size = sizeof(co_map),
    .superclass = 0,
    .init = co_map_init,
    .destructor = co_map_dealloc,
    .hash = 0,
    .equal = 0
};


void* cmap_get(co_map_t map,uint64_t key)
{
    struct hlist_head *head = &map->table[key % MAP_CAPACITY];
    struct hlist_node *p = 0;
    void *obj = 0;
    if(key < 0) return 0;

    read_lock(&map->lock);
    hlist_for_each(p,head)
    {
        cbase_t base = hlist_entry(p,cbase,node);
        if(cobject_hash(base) == key)
        {
            obj = co_retain(base);
            break;
        }
    }
    read_unlock(&map->lock);
    return obj;
}
bool cmap_put(co_map_t map,void* obj)
{
    uint64_t hash = cobject_hash(obj);
    void *tmp = cmap_get(map,hash);
    struct hlist_head *head = 0;
    cbase_t base = 0;
    if(tmp)
    {
        co_release(tmp);
        return false;
    }

    co_retain(obj);
    head = &map->table[hash % MAP_CAPACITY];
    base = (cbase_t)obj;

    write_lock(&map->lock);
    hlist_add_head(&base->node,head);
    map->count += 1;
    write_unlock(&map->lock);
    return true;
}
void cmap_remove(co_map_t map,uint64_t key)
{
    struct hlist_head *head = &map->table[key % MAP_CAPACITY];
    struct hlist_node *p = 0,*n = 0;

    if(key < 0) return;

    write_lock(&map->lock);
    hlist_for_each_safe(p,n,head)
    {
        cbase_t base = hlist_entry(p,cbase,node);
        if(cobject_hash(base) == key)
        {
            map->count -= 1;
            hlist_del(&base->node);
            co_release(base);
        }
    }
    write_unlock(&map->lock);
}
void cmap_empty(co_map_t map)
{
    int i = 0;
    write_lock(&map->lock);
    for (i = 0; i < MAP_CAPACITY; i++)
    {
        struct hlist_head *head = &map->table[i];
        struct hlist_node *p = 0,*n = 0;
        hlist_for_each_safe(p,n,head)
        {
            cbase_t base = hlist_entry(p,cbase,node);
            hlist_del(&base->node);
            co_release(base);
        }
    }
    map->count = 0;
    write_unlock(&map->lock);
}

uint64_t cmap_count(co_map_t map)
{
    uint64_t count = 0;
    read_lock(&map->lock);
    count = map->count;
    read_unlock(&map->lock);
    return count;
}

void cmap_enumeration(co_map_t map,cmap_enum_func cb,bool monopoly,void *udata)
{
    if(!cb) return;
    int i = 0;
    if(monopoly) write_lock(&map->lock);
    else read_lock(&map->lock);
    for (i = 0; i < MAP_CAPACITY; i++)
    {
        struct hlist_head *head = &map->table[i];
        struct hlist_node *p = 0,*n = 0;
        bool stop = false;
        hlist_for_each_safe(p,n,head)
        {
            bool delete = false;
            cbase_t base = hlist_entry(p,cbase,node);
            cb(map,(void *)cobject_hash(base),base,&stop,&delete,udata);
            if(delete && monopoly)
            {
                map->count--;
                hlist_del(&base->node); 
                co_release(base);
            }
            if(stop) break;
        }
        if(stop) break;
    }
    if(monopoly) write_unlock(&map->lock);
    else read_unlock(&map->lock);
}