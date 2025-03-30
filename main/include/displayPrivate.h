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

void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb,
    int32_t start, int32_t end,
    uint16_t time,
    uint32_t delay,
    lv_anim_ready_cb_t ready_cb,
    lv_anim_path_cb_t path_cb
);


//Bsp

void Backlight_SetGradual(uint16_t target, uint16_t time);