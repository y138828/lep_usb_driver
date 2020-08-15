
#include "cobjc.h"

static void _co_init(void *aself, cclass *aclass)
{
	if (!aclass)
		return;

	_co_init(aself, aclass->superclass);

	if (aclass->init)
		aclass->init(aself);
}


void *co_create(cclass *aclass)
{
	cbase *instance = kzalloc(aclass->size,GFP_ATOMIC);
	if(!instance) return 0;
	instance->aclass          = aclass;
	INIT_HLIST_NODE(&instance->node);
	atomic_set(&instance->reference_count,1);

	_co_init(instance,aclass);
	return instance;
}


void *co_retain(void *aself)
{
	cbase *self = aself;
	if(!self)
		return aself;
    atomic_add(1,&self->reference_count);
	return aself;
}

static void _co_destroy_class(void *aself, cclass *aclass)
{
	if (!aclass)
		return;

	if (aclass->destructor)
		aclass->destructor(aself);

	_co_destroy_class(aself, aclass->superclass);
}

void co_release(void *aself)
{
	cbase *self = aself;
	if (!self)
		return;

    if(atomic_sub_and_test(1,&self->reference_count))
    {
        _co_destroy_class(self, self->aclass);
        kfree(self);
    }
}


uint64_t cobject_hash(void *obj)
{
	cbase_t object =(cbase_t)obj;
	cclass_t cls = object->aclass;
	while (cls)
	{
		if(cls->hash) return cls->hash(obj);
		cls = cls->superclass;
	}
	
	return 0;
}
bool cobject_equal(void* obj1,void*obj2)
{
	cbase_t object1 =(cbase_t)obj1;
	cbase_t object2 =(cbase_t)obj2;
	cclass_t cls = object1->aclass;
	if(object1->aclass != object2->aclass) return false;
	while (cls)
	{
		if(cls->equal) return cls->equal(obj1,obj2);
		cls = cls->superclass;
	}
	return false;
}