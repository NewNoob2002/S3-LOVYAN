#pragma once

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "LGFX_ST7789.h"

#include "ui.h"

#define LV_TICK_PERIOD_MS 1

#define TFT_WIDTH 320
#define TFT_HEIGHT 172

extern LGFX_ST7789 lcd;

/*LittleVGL*/
#include "lvgl.h"
#define LV_ANIM_TIME_DEFAULT 200


esp_err_t lv_display_init();
void lvgl_acquire(void);
void lvgl_release(void);