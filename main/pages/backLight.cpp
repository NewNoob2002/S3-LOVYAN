#include "displayPrivate.h"

static void Backlight_AnimCallback(void *obj, uint8_t brightness)
{
    lcd.setBrightness(brightness);
}

uint8_t Backlight_GetValue()
{
    return lcd.getBrightness();
}

void Backlight_SetGradual(uint16_t target, uint16_t time)
{
    static lv_anim_t a;
    lv_obj_add_anim(
        NULL, &a,
        (lv_anim_exec_xcb_t)Backlight_AnimCallback,
        Backlight_GetValue(), target,
        time, NULL, NULL);
}