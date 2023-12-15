/**
* @file lv_cache.c
*
*/

/*********************
 *      INCLUDES
 *********************/
#include "lv_cache.h"
#include "_lv_cache_entry_internal.h"
#include "../lv_assert.h"
#include "../../stdlib/lv_sprintf.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_cache_t_ * lv_cache_create_(const lv_cache_class_t * cache_class,
                               size_t node_size, size_t max_size,
                               lv_cache_compare_cb_t compare_cb,
                               lv_cache_create_cb_t create_cb,
                               lv_cache_free_cb_t free_cb)
{
    lv_cache_t_ * cache = cache_class->alloc_cb();
    LV_ASSERT_MALLOC(cache);

    cache->clz = cache_class;
    cache->node_size = node_size;
    cache->max_size = max_size;
    cache->size = 0;
    cache->compare_cb = compare_cb;
    cache->create_cb = create_cb;
    cache->free_cb = free_cb;

    cache->clz->init_cb(cache);

    lv_mutex_init(&cache->lock);

    return cache;
}

void   lv_cache_destroy_(lv_cache_t_ * cache, void * user_data)
{
    LV_ASSERT_NULL(cache);
    cache->clz->destroy_cb(cache, user_data);
    lv_mutex_delete(&cache->lock);
}

lv_cache_entry_t_ * lv_cache_get_(lv_cache_t_ * cache, const void * key, void * user_data)
{
    LV_ASSERT_NULL(cache);
    LV_ASSERT_NULL(key);

    lv_mutex_lock(&cache->lock);
    lv_cache_entry_t_ * entry = cache->clz->get_cb(cache, key, user_data);
    lv_mutex_unlock(&cache->lock);
    return entry;
}
lv_cache_entry_t_ * lv_cache_get_or_create_(lv_cache_t_ * cache, const void * key, void * user_data)
{
    LV_ASSERT_NULL(cache);
    LV_ASSERT_NULL(key);

    lv_mutex_lock(&cache->lock);
    lv_cache_entry_t_ * entry = cache->clz->get_cb(cache, key, user_data);
    if(entry == NULL) {
        entry = cache->clz->create_entry_cb(cache, key, user_data);
    }
    lv_mutex_unlock(&cache->lock);
    return entry;
}
void   lv_cache_drop_(lv_cache_t_ * cache, const void * key, void * user_data)
{
    LV_ASSERT_NULL(cache);
    LV_ASSERT_NULL(key);

    lv_mutex_lock(&cache->lock);
    lv_cache_entry_t_ * entry = cache->clz->get_cb(cache, key, user_data);
    if(entry == NULL) {
        lv_mutex_unlock(&cache->lock);
        return;
    }

    if(lv_cache_entry_ref_get(entry) == 0) {
        cache->clz->drop_cb(cache, key, user_data);
    }
    else {
        lv_cache_entry_set_invalid(entry, true);
        lv_cache_remove_(cache, entry, user_data);
    }
    lv_mutex_unlock(&cache->lock);
}
void   lv_cache_remove_(lv_cache_t_* cache, lv_cache_entry_t_* entry, void * user_data)
{
    LV_ASSERT_NULL(cache);
    LV_ASSERT_NULL(entry);

    lv_mutex_lock(&cache->lock);
    cache->clz->remove_cb(cache, entry, user_data);
    lv_mutex_unlock(&cache->lock);
}
void   lv_cache_drop_all_(lv_cache_t_ * cache, void * user_data)
{
    LV_ASSERT_NULL(cache);

    lv_mutex_lock(&cache->lock);
    cache->clz->drop_all_cb(cache, user_data);
    lv_mutex_unlock(&cache->lock);
}
void   lv_cache_set_max_size_(lv_cache_t_ * cache, size_t max_size, void * user_data)
{
    LV_UNUSED(user_data);
    cache->max_size = max_size;
}
size_t lv_cache_get_max_size_(lv_cache_t_ * cache, void * user_data)
{
    LV_UNUSED(user_data);
    return cache->max_size;
}
size_t lv_cache_get_size_(lv_cache_t_ * cache, void * user_data)
{
    LV_UNUSED(user_data);
    return cache->size;
}
size_t lv_cache_get_free_size_(lv_cache_t_ * cache, void * user_data)
{
    LV_UNUSED(user_data);
    return cache->max_size - cache->size;
}
void   lv_cache_set_compare_cb(lv_cache_t_ * cache, lv_cache_compare_cb_t compare_cb, void * user_data)
{
    LV_UNUSED(user_data);
    cache->compare_cb = compare_cb;
}
void   lv_cache_set_create_cb(lv_cache_t_ * cache, lv_cache_create_cb_t alloc_cb, void * user_data)
{
    LV_UNUSED(user_data);
    cache->create_cb = alloc_cb;
}
void   lv_cache_set_free_cb(lv_cache_t_ * cache, lv_cache_free_cb_t free_cb, void * user_data)
{
    LV_UNUSED(user_data);
    cache->free_cb = free_cb;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
