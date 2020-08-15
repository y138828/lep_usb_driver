#ifndef _HEAD_CCLASSD_H
#define _HEAD_CCLASSD_H


#define LEPMETA_DEFINE_META_INIT_HEAD(name) static void* lep_ ## name ## _init(void *aself)
#define LEPMETA_DEFINE_META_DALLOC_HEAD(name) static void lep_ ## name ## _dalloc(void *aself)
#define LEPMETA_DEFINE_META_INIT_EMPTY(name) LEPMETA_DEFINE_META_INIT_HEAD(name){ return aself; }
#define LEPMETA_DEFINE_META_DALLOC_EMPTY(name) LEPMETA_DEFINE_META_DALLOC_HEAD(name){  }
#define LEPMETA_DEFINE_META_HASH(name,type,imp,offset) static uint64_t lep_ ## name ## _hash(void *aself){ type self = (type)aself; return imp(&(self->offset)); }
#define LEPMETA_DEFINE_META_EQAL(name,type,imp,offset) static bool lep_ ## name ## _eqal(void *obj1,void*obj2){ type left = (type)obj1;type right = (type)obj2; return imp(&(left->offset),&(right->offset)); }


#define LEPMETA_DEFINE_META(name,ssize,supper) cclass lep_ ## name ## _meta_class = { \
    .size = ssize, \
    .superclass = supper, \
    .init = lep_## name ## _init, \
    .destructor = lep_ ## name ## _dalloc, \
    .hash = lep_ ## name ## _hash, \
    .equal = lep_ ## name ## _eqal \
};


typedef struct _rules_enum_for_copy_inline_udata{
    char *buf;
    int len;
    int used_len;
    int count;
}rules_enum_for_copy_inline_udata,*rules_enum_for_copy_inline_udata_t;
typedef struct _rules_enum_for_select_udata{
    void* uinfo;
    void* uinfo2;
    void *select;
}rules_enum_for_select_udata,*rules_enum_for_select_udata_t;

#define LEPCLASS_IMP_RULE_ADD(name,type,otype,meta_name,type_size,map) LEPSPRULE_DEFINE_ADD(name,type) \
{ \
    otype rule = co_create(&meta_name); \
    bool ret = false; \
    if(!rule) return false; \
    memcpy(&rule->rule,data,type_size); \
    ret = cmap_put(map,rule); \
    co_release(rule); \
    return ret; \
}
#define LEPCLASS_IMP_RULE_ADD_RET(name,type,otype,meta_name,type_size,map) LEPSPRULE_DEFINE_ADD_RET(name,type,otype) \
{ \
    otype rule = co_create(&meta_name); \
    if(!rule) return 0; \
    memcpy(&rule->rule,data,type_size); \
    bool ret = cmap_put(map,rule); \
    if (ret) { return rule; } \
    else{ co_release(rule);return 0; } \
}

#define LEPCLASS_IMP_RULE_REMOVE(name,type,imp,map) LEPSPRULE_DEFINE_REMOVE(name,type) \
{ \
    uint64_t key = imp(target); \
    cmap_remove(map,key); \
}
#define LEPCLASS_IMP_RULE_REMOVE_ID(name,imp,map) LEPSPRULE_DEFINE_REMOVE_ID(name) {return cmap_enumeration(map,imp,true,(void *)id);}
#define LEPCLASS_IMP_RULE_REMOVE_ALL(name,map) LEPSPRULE_DEFINE_REMOVE_ALL(name) {return cmap_empty(map);}
#define LEPCLASS_IMP_RULE_COPY(name,imp,map) LEPSPRULE_DEFINE_COPY_ALL(name) \
{ \
    rules_enum_for_copy_inline_udata udata = {0}; \
    udata.buf = buf; \
    udata.len = *len; \
    udata.used_len = sizeof(uint32_t); \
    udata.count = 0; \
    cmap_enumeration(map,imp,false,(void *)&udata); \
    *len = udata.used_len; \
    *((uint32_t *)buf) = udata.count; \
    if(udata.used_len == 0 ) return -EIO; \
    return udata.used_len; \
}
#define LEPCLASS_IMP_RULE_HIT(name,type,rtype,map,imp) LEPSPRULE_DEFINE_HIT(name,type,rtype) {return cmap_get(map,imp(target));}

//extend

#define LEPSPRULE_DEFINE_ADD(name,type) bool add_ ## name ## _rule(type data)
#define LEPSPRULE_DEFINE_ADD_RET(name,type,rtype) rtype add_ ## name ## _rule_ret(type data)
#define LEPSPRULE_DEFINE_REMOVE(name,type) void remove_ ## name ## _rule(type target)
#define LEPSPRULE_DEFINE_REMOVE_ID(name) void remove_ ## name ## _rule_with_id(uint64_t id)
#define LEPSPRULE_DEFINE_REMOVE_ALL(name) void remove_all_ ## name ## _rule(void)
#define LEPSPRULE_DEFINE_COPY_ALL(name) int copy_all_ ## name ## _rule(uint8_t *buf,int *len)
#define LEPSPRULE_DEFINE_HIT(name,type,rtype) rtype hit_ ## name ## _rule(type target)


#endif