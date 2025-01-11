/**
 * @file lv_theme_basic.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h" /*To see all the widgets*/

#if LV_USE_THEME_BASIC

#include "lv_theme_basic.h"
#include "../../../misc/lv_gc.h"

/*********************
 *      DEFINES
 *********************/
#define COLOR_SCR       lv_color_make(0, 4, 8)
#define COLOR_PANNEL    lv_color_hex(0xA5A5A5)
#define COLOR_BORDER    lv_color_hex(0xA5A5A5)

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_style_t scr;
    lv_style_t pannel;
    lv_style_t label;
} my_theme_styles_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void style_init_reset(lv_style_t * style);
static void theme_apply(lv_theme_t * th, lv_obj_t * obj);

/**********************
 *  STATIC VARIABLES
 **********************/
static my_theme_styles_t * styles;
static lv_theme_t theme;
static bool inited;

/**********************
 *      MACROS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void style_init(void)
{
    //scr
    style_init_reset(&styles->scr);
    lv_style_set_bg_opa(&styles->scr, LV_OPA_COVER);
    lv_style_set_bg_color(&styles->scr, COLOR_SCR);

    //pannel
    style_init_reset(&styles->pannel);
    lv_style_set_bg_opa(&styles->pannel, LV_OPA_COVER);
    lv_style_set_bg_color(&styles->pannel, COLOR_PANNEL);

    //label
    style_init_reset(&styles->label);
    lv_style_set_text_opa(&styles->label, LV_OPA_COVER);
    lv_style_set_text_color(&styles->label, lv_color_white());
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

bool lv_theme_basic_is_inited(void)
{
    return  LV_GC_ROOT(_lv_theme_basic_styles) == NULL ? false : true;
}

lv_theme_t * lv_theme_basic_init(lv_disp_t * disp)
{

    /*This trick is required only to avoid the garbage collection of
     *styles' data if LVGL is used in a binding (e.g. Micropython)
     *In a general case styles could be in simple `static lv_style_t my_style...` variables*/
    if(!lv_theme_basic_is_inited()) {
        inited = false;
        LV_GC_ROOT(_lv_theme_basic_styles) = lv_mem_alloc(sizeof(my_theme_styles_t));
        styles = (my_theme_styles_t *)LV_GC_ROOT(_lv_theme_basic_styles);
    }

    theme.disp = disp;
    theme.font_small = LV_FONT_DEFAULT;
    theme.font_normal = LV_FONT_DEFAULT;
    theme.font_large = LV_FONT_DEFAULT;
    theme.apply_cb = theme_apply;

    style_init();

    if(disp == NULL || lv_disp_get_theme(disp) == &theme) {
        lv_obj_report_style_change(NULL);
    }

    inited = true;

    return (lv_theme_t *)&theme;
}

lv_theme_t * lv_theme_basic_get(void)
{
    if(!lv_theme_basic_is_inited()) {
        return NULL;
    }

    return (lv_theme_t *)&theme;
}

static void theme_apply(lv_theme_t * th, lv_obj_t * obj)
{
    LV_UNUSED(th);

    if(lv_obj_get_parent(obj) == NULL) {
        lv_obj_add_style(obj, &styles->scr, LV_PART_MAIN | LV_STATE_DEFAULT);
        return;
    }

    if(lv_obj_check_type(obj, &lv_obj_class)) {
        lv_obj_add_style(obj, &styles->pannel, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if(lv_obj_check_type(obj, &lv_label_class)) {
        lv_obj_add_style(obj, &styles->label, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void style_init_reset(lv_style_t * style)
{
    if(inited) {
        lv_style_reset(style);
    }
    else {
        lv_style_init(style);
    }
}

#endif
