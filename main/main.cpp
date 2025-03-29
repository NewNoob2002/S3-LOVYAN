#include <stdio.h>

#include <LovyanGFX.hpp>

#include "lv_port_disp.hpp"

#include "lv_demos.h"
extern "C" void app_main(void)
{
    lcd.init();        // Initialize LovyanGFX
    lv_init();         // Initialize lvgl
    if (lv_display_init() != ESP_OK) // Configure LVGL
    {
        ESP_LOGE(TAG, "LVGL setup failed!!!");
    }

    lv_demo_stress();
}