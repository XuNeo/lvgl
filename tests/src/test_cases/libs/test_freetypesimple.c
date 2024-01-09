#if LV_BUILD_TEST
#include "../lvgl.h"

#include "unity/unity.h"

#if __WORDSIZE == 64
    #define TEST_FREETYPE_ASSERT_EQUAL_SCREENSHOT(NAME) TEST_ASSERT_EQUAL_SCREENSHOT("libs/freetype_simple" NAME ".lp64.png")
#elif __WORDSIZE == 32
    #define TEST_FREETYPE_ASSERT_EQUAL_SCREENSHOT(NAME) TEST_ASSERT_EQUAL_SCREENSHOT("libs/freetype_simple" NAME ".lp32.png")
#endif

void setUp(void)
{
    /* Function run before every test */
}

void tearDown(void)
{
    /* Function run after every test */
}

void test_freetype_bitmap_rendering_test(void)
{
#if LV_USE_FREETYPE
    /*Create a font*/
    lv_font_t * font_italic = lv_freetype_font_create("./src/test_files/fonts/noto/NotoSansSC-Regular.ttf",
                                                      LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                                                      24,
                                                      LV_FREETYPE_FONT_STYLE_NORMAL);

    /*Create a label with the new style*/
    lv_obj_t * label0 = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(label0, font_italic, 0);
    lv_obj_set_width(label0, lv_obj_get_width(lv_screen_active()) - 20);
    lv_label_set_text(label0, "人");
    lv_obj_align(label0,  LV_ALIGN_TOP_LEFT, 0, 10);

    TEST_FREETYPE_ASSERT_EQUAL_SCREENSHOT("1");
#else
    TEST_PASS();
#endif
}

#endif
