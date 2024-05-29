/**
 * @file lv_obj_id.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../core/lv_obj.h"
#include "../stdlib/lv_string.h"
#include "lv_obj_property.h"

#if LV_USE_OBJ_PROPERTY

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef void (*lv_property_set_int_t)(lv_obj_t *, int32_t);
typedef void (*lv_property_set_pointer_t)(lv_obj_t *, const void *);
typedef void (*lv_property_set_color_t)(lv_obj_t *, lv_color_t);
typedef lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, const lv_property_t *);

typedef int32_t (*lv_property_get_int_t)(const lv_obj_t *);
typedef void * (*lv_property_get_pointer_t)(const lv_obj_t *);
typedef lv_color_t (*lv_property_get_color_t)(const lv_obj_t *);
typedef lv_result_t (*lv_property_getter_t)(const lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * value, bool set);
static lv_result_t obj_property_by_name(lv_obj_t * obj, const char * name, lv_property_t * value, bool set);

/**********************
 *  STATIC VARIABLES
 **********************/

static const struct _style_names {
    uint32_t id;
    const char * name;
} g_style_names[] = {
    { LV_STYLE_WIDTH, "width"},
    { LV_STYLE_HEIGHT, "height"},
    { LV_STYLE_LENGTH, "length"},
    { LV_STYLE_MIN_WIDTH, "min_width" },
    { LV_STYLE_MAX_WIDTH, "max_width" },
    { LV_STYLE_MIN_HEIGHT, "min_height" },
    { LV_STYLE_MAX_HEIGHT, "max_height" },
    { LV_STYLE_X, "x" },
    { LV_STYLE_Y, "y" },
    { LV_STYLE_ALIGN, "align" },
    { LV_STYLE_RADIUS, "radius" },
    { LV_STYLE_PAD_TOP, "pad_top" },
    { LV_STYLE_PAD_BOTTOM, "pad_bottom" },
    { LV_STYLE_PAD_LEFT, "pad_left" },
    { LV_STYLE_PAD_RIGHT, "pad_right" },
    { LV_STYLE_PAD_ROW, "pad_row" },
    { LV_STYLE_PAD_COLUMN, "pad_column" },
    { LV_STYLE_LAYOUT, "layout" },
    { LV_STYLE_MARGIN_TOP, "margin_top" },
    { LV_STYLE_MARGIN_BOTTOM, "margin_bottom" },
    { LV_STYLE_MARGIN_LEFT, "margin_left" },
    { LV_STYLE_MARGIN_RIGHT, "margin_right" },
    { LV_STYLE_BG_COLOR, "bg_color" },
    { LV_STYLE_BG_OPA, "bg_opa" },
    { LV_STYLE_BG_GRAD_DIR, "bg_grad_dir" },
    { LV_STYLE_BG_MAIN_STOP, "bg_main_stop" },
    { LV_STYLE_BG_GRAD_STOP, "bg_grad_stop" },
    { LV_STYLE_BG_GRAD_COLOR, "bg_grad_color" },
    { LV_STYLE_BG_MAIN_OPA, "bg_main_opa" },
    { LV_STYLE_BG_GRAD_OPA, "bg_grad_opa" },
    { LV_STYLE_BG_GRAD, "bg_grad" },
    { LV_STYLE_BASE_DIR, "base_dir" },
    { LV_STYLE_BG_IMAGE_SRC, "bg_image_src" },
    { LV_STYLE_BG_IMAGE_OPA, "bg_image_opa" },
    { LV_STYLE_BG_IMAGE_RECOLOR, "bg_image_recolor" },
    { LV_STYLE_BG_IMAGE_RECOLOR_OPA, "bg_image_recolor_opa" },
    { LV_STYLE_BG_IMAGE_TILED, "bg_image_tiled" },
    { LV_STYLE_CLIP_CORNER, "clip_corner" },
    { LV_STYLE_BORDER_WIDTH, "border_width" },
    { LV_STYLE_BORDER_COLOR, "border_color" },
    { LV_STYLE_BORDER_OPA, "border_opa" },
    { LV_STYLE_BORDER_SIDE, "border_side" },
    { LV_STYLE_BORDER_POST, "border_post" },
    { LV_STYLE_OUTLINE_WIDTH, "outline_width" },
    { LV_STYLE_OUTLINE_COLOR, "outline_color" },
    { LV_STYLE_OUTLINE_OPA, "outline_opa" },
    { LV_STYLE_OUTLINE_PAD, "outline_pad" },
    { LV_STYLE_SHADOW_WIDTH, "shadow_width" },
    { LV_STYLE_SHADOW_COLOR, "shadow_color" },
    { LV_STYLE_SHADOW_OPA, "shadow_opa" },
    { LV_STYLE_SHADOW_OFFSET_X, "shadow_offset_x" },
    { LV_STYLE_SHADOW_OFFSET_Y, "shadow_offset_y" },
    { LV_STYLE_SHADOW_SPREAD, "shadow_spread" },
    { LV_STYLE_IMAGE_OPA, "image_opa" },
    { LV_STYLE_IMAGE_RECOLOR, "image_recolor" },
    { LV_STYLE_IMAGE_RECOLOR_OPA, "image_recolor_opa" },
    { LV_STYLE_LINE_WIDTH, "line_width" },
    { LV_STYLE_LINE_DASH_WIDTH, "line_dash_width" },
    { LV_STYLE_LINE_DASH_GAP, "line_dash_gap" },
    { LV_STYLE_LINE_ROUNDED, "line_rounded" },
    { LV_STYLE_LINE_COLOR, "line_color" },
    { LV_STYLE_LINE_OPA, "line_opa" },
    { LV_STYLE_ARC_WIDTH, "arc_width" },
    { LV_STYLE_ARC_ROUNDED, "arc_rounded" },
    { LV_STYLE_ARC_COLOR, "arc_color" },
    { LV_STYLE_ARC_OPA, "arc_opa" },
    { LV_STYLE_ARC_IMAGE_SRC, "arc_image_src" },
    { LV_STYLE_TEXT_COLOR, "text_color" },
    { LV_STYLE_TEXT_OPA, "text_opa" },
    { LV_STYLE_TEXT_FONT, "text_font" },
    { LV_STYLE_TEXT_LETTER_SPACE, "text_letter_space" },
    { LV_STYLE_TEXT_LINE_SPACE, "text_line_space" },
    { LV_STYLE_TEXT_DECOR, "text_decor" },
    { LV_STYLE_TEXT_ALIGN, "text_align" },
    { LV_STYLE_OPA, "opa" },
    { LV_STYLE_OPA_LAYERED, "opa_layered" },
    { LV_STYLE_COLOR_FILTER_DSC, "color_filter_dsc" },
    { LV_STYLE_COLOR_FILTER_OPA, "color_filter_opa" },
    { LV_STYLE_ANIM, "anim" },
    { LV_STYLE_ANIM_DURATION, "anim_duration" },
    { LV_STYLE_TRANSITION, "transition" },
    { LV_STYLE_BLEND_MODE, "blend_mode" },
    { LV_STYLE_TRANSFORM_WIDTH, "transform_width" },
    { LV_STYLE_TRANSFORM_HEIGHT, "transform_height" },
    { LV_STYLE_TRANSLATE_X, "translate_x" },
    { LV_STYLE_TRANSLATE_Y, "translate_y" },
    { LV_STYLE_TRANSFORM_SCALE_X, "transform_scale_x" },
    { LV_STYLE_TRANSFORM_SCALE_Y, "transform_scale_y" },
    { LV_STYLE_TRANSFORM_ROTATION, "transform_rotation" },
    { LV_STYLE_TRANSFORM_PIVOT_X, "transform_pivot_x" },
    { LV_STYLE_TRANSFORM_PIVOT_Y, "transform_pivot_y" },
    { LV_STYLE_TRANSFORM_SKEW_X, "transform_skew_x" },
    { LV_STYLE_TRANSFORM_SKEW_Y, "transform_skew_y" },
    { LV_STYLE_BITMAP_MASK_SRC, "bitmap_mask_src" },
    { LV_STYLE_ROTARY_SENSITIVITY, "rotary_sensitivity" },
#if LV_USE_FLEX
    { LV_STYLE_FLEX_FLOW, "flex_flow" },
    { LV_STYLE_FLEX_MAIN_PLACE, "flex_main_place" },
    { LV_STYLE_FLEX_CROSS_PLACE, "flex_cross_place" },
    { LV_STYLE_FLEX_TRACK_PLACE, "flex_track_place" },
    { LV_STYLE_FLEX_GROW, "flex_grow" },
#endif
#if LV_USE_GRID
    { LV_STYLE_GRID_COLUMN_ALIGN, "grid_column_align" },
    { LV_STYLE_GRID_ROW_ALIGN, "grid_row_align" },
    { LV_STYLE_GRID_ROW_DSC_ARRAY, "grid_row_dsc_array" },
    { LV_STYLE_GRID_COLUMN_DSC_ARRAY, "grid_column_dsc_array" },
    { LV_STYLE_GRID_CELL_COLUMN_POS, "grid_cell_column_pos" },
    { LV_STYLE_GRID_CELL_COLUMN_SPAN, "grid_cell_column_span" },
    { LV_STYLE_GRID_CELL_X_ALIGN, "grid_cell_x_align" },
    { LV_STYLE_GRID_CELL_ROW_POS, "grid_cell_row_pos" },
    { LV_STYLE_GRID_CELL_ROW_SPAN, "grid_cell_row_span" },
    { LV_STYLE_GRID_CELL_Y_ALIGN, "grid_cell_y_align" },
#endif
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, const lv_property_t * value)
{
    LV_ASSERT(obj && value);

    if(value->id == LV_PROPERTY_ID_INVALID) {
        LV_LOG_WARN("invalid property id set to %p\n", obj);
        return LV_RESULT_INVALID;
    }

    if(value->id < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, value->id, value->_style, 0);
        return LV_RESULT_OK;
    }

    return obj_property(obj, value->id, (lv_property_t *)value, true);
}

lv_result_t lv_obj_set_property_by_name(lv_obj_t * obj, const char * name, const lv_property_t * value)
{
    LV_ASSERT(obj && name);

    /*Check if name is a style */
    for (unsigned i = 0; i < sizeof(g_style_names) / sizeof(g_style_names[0]); i++) {
        if (lv_strcmp(name, g_style_names[i].name) == 0) {
            lv_obj_set_local_style_prop(obj, g_style_names[i].id, value->_style, 0);
            return LV_RESULT_OK;
        }
    }

    return obj_property_by_name(obj, name, (lv_property_t *)value, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, const lv_property_t * value, uint32_t count)
{
    for(uint32_t i = 0; i < count; i++) {
        lv_result_t result = lv_obj_set_property(obj, &value[i]);
        if(result != LV_RESULT_OK) {
            return result;
        }
    }

    return LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    lv_result_t result;
    lv_property_t value;

    if(id == LV_PROPERTY_ID_INVALID) {
        LV_LOG_WARN("invalid property id to get from %p\n", obj);
        value.id = 0;
        value.num = 0;
        return value;
    }

    if(id < LV_PROPERTY_ID_START) {
        lv_obj_get_local_style_prop(obj, id, &value._style, 0);
        value.id = id;
        return value;
    }

    result = obj_property(obj, id, &value, false);
    if(result != LV_RESULT_OK)
        value.id = 0;

    return value;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

static lv_result_t obj_property_by_name(lv_obj_t * obj, const char * name, lv_property_t * value, bool set)
{
    const lv_property_ops_t * properties;
    const lv_property_ops_t * prop;

    const lv_obj_class_t * clz;

    for(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        if(properties == NULL) {
            /* try base class*/
            continue;
        }

        /*Check if there's setter available for this class*/
        for(uint32_t i = 0; i < clz->properties_count; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            if(prop->name && prop->name[0] == '*') {
                value->id = prop->id;
                if(set) return ((lv_property_setter_t)prop->setter)(obj, prop->id, value);
                else return ((lv_property_getter_t)prop->getter)(obj, prop->id, value);
            }

            /*Not this id, check next*/
            if(lv_strcmp(name, prop->name) != 0)
                continue;

            /*name matched but we got null pointer to functions*/
            if(set ? prop->setter == NULL : prop->getter == NULL) {
                LV_LOG_WARN("null %s provided, id: %s\n", set ? "setter" : "getter", name);
                return LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            if(!set) value->id = prop->id;

            switch(LV_PROPERTY_ID_TYPE(prop->id)) {
                case LV_PROPERTY_TYPE_INT:
                    if(set)((lv_property_set_int_t)(prop->setter))(obj, value->num);
                    else value->num = ((lv_property_get_int_t)(prop->getter))(obj);
                    break;
                case LV_PROPERTY_TYPE_POINTER:
                case LV_PROPERTY_TYPE_IMGSRC:
                    if(set)((lv_property_set_pointer_t)(prop->setter))(obj, value->ptr);
                    else value->ptr = ((lv_property_get_pointer_t)(prop->getter))(obj);
                    break;
                case LV_PROPERTY_TYPE_COLOR:
                    if(set)((lv_property_set_color_t)prop->setter)(obj, value->color);
                    else value->color = ((lv_property_get_color_t)(prop->getter))(obj);
                    break;
                default:
                    LV_LOG_WARN("unknown property id: 0x%08x\n", prop->id);
                    return LV_RESULT_INVALID;
                    break;
            }

            return LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("unknown property name: %s\n", name);
    return LV_RESULT_INVALID;
}


static lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * value, bool set)
{
    const lv_property_ops_t * properties;
    const lv_property_ops_t * prop;

    const lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    for(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        if(properties == NULL) {
            /* try base class*/
            continue;
        }

        if(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            continue;
        }

        /*Check if there's setter available for this class*/
        for(uint32_t i = 0; i < clz->properties_count; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            if(prop->id == LV_PROPERTY_ID_ANY) {
                value->id = prop->id;
                if(set) return ((lv_property_setter_t)prop->setter)(obj, id, value);
                else return ((lv_property_getter_t)prop->getter)(obj, id, value);
            }

            /*Not this id, check next*/
            if(prop->id != id)
                continue;

            /*id matched but we got null pointer to functions*/
            if(set ? prop->setter == NULL : prop->getter == NULL) {
                LV_LOG_WARN("null %s provided, id: %d\n", set ? "setter" : "getter", id);
                return LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            if(!set) value->id = prop->id;

            switch(LV_PROPERTY_ID_TYPE(prop->id)) {
                case LV_PROPERTY_TYPE_INT:
                    if(set)((lv_property_set_int_t)(prop->setter))(obj, value->num);
                    else value->num = ((lv_property_get_int_t)(prop->getter))(obj);
                    break;
                case LV_PROPERTY_TYPE_POINTER:
                case LV_PROPERTY_TYPE_IMGSRC:
                    if(set)((lv_property_set_pointer_t)(prop->setter))(obj, value->ptr);
                    else value->ptr = ((lv_property_get_pointer_t)(prop->getter))(obj);
                    break;
                case LV_PROPERTY_TYPE_COLOR:
                    if(set)((lv_property_set_color_t)prop->setter)(obj, value->color);
                    else value->color = ((lv_property_get_color_t)(prop->getter))(obj);
                    break;
                default:
                    LV_LOG_WARN("unknown property id: 0x%08x\n", prop->id);
                    return LV_RESULT_INVALID;
                    break;
            }

            return LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("unknown property id: 0x%08x\n", id);
    return LV_RESULT_INVALID;
}

#endif /*LV_USE_OBJ_PROPERTY*/
