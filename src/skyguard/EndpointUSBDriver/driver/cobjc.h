#include <linux/kernel.h>
#include <linux/atomic.h>
#include <linux/slab.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

#ifndef _HEAD_COBJC_H
#define _HEAD_COBJC_H

typedef void* (*co_init)(void *aself);
typedef void (*co_destructor)(void *aself);
typedef uint64_t (*co_hash)(void *obj);
typedef bool (*co_equal)(void *obj1,void*obj2);

typedef struct  _co_class
{
    struct _co_class *superclass;
	size_t size;
	co_init init;
	co_destructor destructor;
	co_hash hash;
	co_equal equal;
}cclass,*cclass_t;

typedef struct _co_cbase
{
	cclass *aclass;
    atomic_t reference_count;
	struct hlist_node node;
}cbase,*cbase_t;

void *co_create(cclass *aclass);

void *co_retain(void *aself);
void co_release(void *aself);
#define co_release_safe(obj)  do { if(obj) co_release(obj);  obj = 0; } while (false);

uint64_t cobject_hash(void *obj);
bool cobject_equal(void* obj1,void*obj2);

#endif