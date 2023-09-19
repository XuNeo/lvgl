/**
 * @file lv_obj_property.h
 *
 */

#ifndef LV_OBJ_PROPERTY_H
#define LV_OBJ_PROPERTY_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../misc/lv_types.h"
#include "lv_style.h"

#if LV_USE_OBJ_PROPERTY

/*********************
 *      DEFINES
 *********************/

/*All possible property value types*/
#define LV_PROPERTY_TYPE_INVALID        0   /*Use default 0 as invalid to detect program outliers*/
#define LV_PROPERTY_TYPE_INT            1   /*int32_t type*/
#define LV_PROPERTY_TYPE_COLOR          2   /*ARGB8888 type*/
#define LV_PROPERTY_TYPE_POINTER        3   /*void * pointer*/
#define LV_PROPERTY_TYPE_IMGSRC         4   /*Special pointer for image*/

/**********************
 *      TYPEDEFS
 **********************/

struct _lv_obj_t;

#define LV_PROPERTY_(clz, name, type, index)    LV_PROPERTY_## clz ##_##name = (LV_PROPERTY_## clz ##_START + (index)) | ((type) << 28)

#define LV_PROPERTY_ID_TYPE(id) ((id) >> 28)
#define LV_PROPERTY_ID_INDEX(id) ((id) & 0xfffffff)

/**
 * Enumeration of all built in widget properties
 *
 * Props are split into groups of 16. When adding a new prop to a group, ensure it does not overflow into the next one.
 */
typedef enum _lv_prop_id_t {
    LV_PROPERTY_ID_INVALID = 0,

    LV_PROPERTY_ID_START = 0x100, /*ID little than 0xff is style ID*/

    /* lv_obj.c */
    LV_PROPERTY_OBJ_START = 1000,
    LV_PROPERTY_(OBJ, PARENT,       LV_PROPERTY_TYPE_POINTER,   1),
    LV_PROPERTY_OBJ_END = 1100,

    /* lv_image.c */
    LV_PROPERTY_IMAGE_START = 1100,
    LV_PROPERTY_(IMAGE, SRC,        LV_PROPERTY_TYPE_POINTER,   1),
    LV_PROPERTY_(IMAGE, OFFSET_X,   LV_PROPERTY_TYPE_INT,       2),
    LV_PROPERTY_(IMAGE, OFFSET_Y,   LV_PROPERTY_TYPE_INT,       3),
    LV_PROPERTY_(IMAGE, ROTATION,   LV_PROPERTY_TYPE_INT,       4),
    LV_PROPERTY_(IMAGE, PIVOT,      LV_PROPERTY_TYPE_POINTER,   5),
    LV_PROPERTY_(IMAGE, SCALE,      LV_PROPERTY_TYPE_INT,       6),
    LV_PROPERTY_(IMAGE, ANTIALIAS,  LV_PROPERTY_TYPE_INT,       7),
    LV_PROPERTY_(IMAGE, SIZE_MODE,  LV_PROPERTY_TYPE_INT,       8),
    LV_PROPERTY_IMAGE_END = 2100,

    /*Special ID*/
    LV_PROPERTY_ID_BUILTIN_LAST, /*Use it to extend ID and make sure it's unique and compile time determinant*/

    LV_PROPERTY_ID_ANY = 0x7ffffffe, /*Special ID used by lvgl to intercept all setter/getter call.*/

    LV_PROPERTY_ID_MAX = 0x7fffffff, /*Makes sure this enum is 32bit wide.*/
} lv_prop_id_t;

typedef struct {
    lv_prop_id_t id;
    union {
        int32_t num;            /**< Number integer number (opacity, enums, booleans or "normal" numbers)*/
        const void * ptr;       /**< Constant pointers  (font, cone text, etc)*/
        lv_color32_t color;     /**< Colors*/
    };
} lv_property_t;

typedef struct {
    lv_prop_id_t id;

    void * setter;
    void * getter;
} lv_property_ops_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*=====================
 * Setter functions
 *====================*/

/**
 * Set where to snap the children when scrolling ends vertically
 * @param obj       pointer to an object
 * @param id        ID of which property
 * @param value     The property value to set
 * @return          return LV_RESULT_OK if success
 */
lv_result_t lv_obj_property_set(const struct _lv_obj_t * obj, const lv_property_t * value);

/*=====================
 * Getter functions
 *====================*/

/**
 * Read property value from object
 * @param obj       pointer to an object
 * @param id        ID of which property
 * @param value     pointer to a buffer to store the value
 * @return ? to be discussed, LV_RESULT_OK or LV_RESULT_INVALID
 */
lv_property_t lv_obj_property_get(const struct _lv_obj_t * obj, lv_prop_id_t id);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_OBJ_PROPERTY*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_PROPERTY_H*/
