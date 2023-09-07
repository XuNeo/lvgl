/**
 * @file lv_obj_id.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_obj.h"
#include <stdint.h>
#include <string.h>

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
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#if LV_USE_OBJID_BUILTIN

void lv_obj_assign_id(lv_obj_t * obj)
{
    LV_ASSERT(obj && obj->class_p);
    LV_ASSERT(obj->id == NULL);
    if (obj->class_p->obj_count == NULL) return;

    uint32_t count = *obj->class_p->obj_count;
    count += 1;
    obj->id = (void*)(lv_uintptr_t)count;
    *obj->class_p->obj_count = count;
}

void lv_obj_free_id(lv_obj_t *obj)
{

}

const char* lv_obj_stringify_id(lv_obj_t * obj, char *buf, uint32_t len)
{
    const char * name;
    LV_ASSERT(obj && obj->class_p);
    if (buf == NULL) return NULL;

#if LV_USE_CLASS_NAME
    name = obj->class_p->name;
#else
    name = "obj";
#endif

    lv_snprintf(buf, len, "%s%" LV_PRIxPTR "", name, (lv_uintptr_t)obj->id);
    return buf;
}

#endif /*LV_USE_OBJID_BUILTIN*/