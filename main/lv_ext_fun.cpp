#include "displayPrivate.h"

void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb, 
    int32_t start, int32_t end,    
    uint16_t time,
    lv_anim_ready_cb_t ready_cb,
    lv_anim_path_cb_t path_cb
)
{
    lv_anim_t a_tmp;
    if(a == NULL)
    {
        lv_anim_init(&a_tmp);
        a = &a_tmp;
    }

    a->var = obj;
    a->start_value = start;
    a->end_value = end;
    a->exec_cb = exec_cb;
    a->path_cb = path_cb;
    a->ready_cb = ready_cb;
    a->time = time;
    lv_anim_start(a);
}