/**
 * @file lv_btn.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_btn.h"
#if LV_USE_BTN != 0

#include "../extra/layouts/flex/lv_flex.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lv_btn_class

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_btn_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);

/**********************
 *  STATIC VARIABLES
 **********************/
#if LV_USE_OBJID_BUILTIN
static uint32_t obj_count = 0;
#endif
const lv_obj_class_t lv_btn_class  = {
    .constructor_cb = lv_btn_constructor,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .instance_size = sizeof(lv_btn_t),
    .base_class = &lv_obj_class,
#if LV_USE_OBJID_BUILTIN
    .obj_count = &obj_count,
#endif
#if LV_USE_CLASS_NAME
    .name = "btn",
#endif
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_btn_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_btn_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    LV_TRACE_OBJ_CREATE("finished");
}

#endif
