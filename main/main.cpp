#include <stdio.h>

#include "displayPrivate.h"

LGFX_ST7789 lcd;

static void anim_x_cb(void *var, int32_t v)
{
    lv_obj_set_x((lv_obj_t *)var, v);
}

static void anim_size_cb(void *var, int32_t v)
{
    lv_obj_set_size((lv_obj_t *)var, v, v);
}

/**
 * Create a playback animation
 */
void lv_example_anim_2(void)
{

    /* Set screen style */
    lv_obj_t *scr = lv_scr_act();
    lv_obj_remove_style_all(scr);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());

    // /* Set root default style */
    // static lv_style_t rootStyle;
    // lv_style_init(&rootStyle);
    // lv_style_set_width(&rootStyle, LV_HOR_RES);
    // lv_style_set_height(&rootStyle, LV_VER_RES);
    // lv_style_set_bg_opa(&rootStyle, LV_OPA_COVER);
    // lv_style_set_bg_color(&rootStyle, lv_color_black());
    // manager.SetRootDefaultStyle(&rootStyle);
}

extern "C" void app_main(void)
{
    lcd.init(); // Initialize LovyanGFX
    lcd.setBrightness(0);
    lv_init();                       // Initialize lvgl
    if (lv_display_init() != ESP_OK) // Configure LVGL
    {
        ESP_LOGE("lvgl", "LVGL setup failed!!!");
    }

    // ui_init();
    lv_example_anim_2();
    vTaskDelay(pdMS_TO_TICKS(100));
    lcd.setBrightness(126);
}