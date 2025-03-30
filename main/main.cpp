#include <stdio.h>

#include "displayPrivate.h"

LGFX_ST7789 lcd;
extern "C" void app_main(void)
{
    lcd.init();        // Initialize LovyanGFX
    lcd.setBrightness(0);
    lv_init();         // Initialize lvgl
    if (lv_display_init() != ESP_OK) // Configure LVGL
    {
        ESP_LOGE("lvgl", "LVGL setup failed!!!");
    }

    ui_init();

    lcd.setBrightness(127);
}