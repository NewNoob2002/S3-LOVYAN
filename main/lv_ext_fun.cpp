#include "displayPrivate.h"

void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb,
    int32_t start, int32_t end,
    uint16_t time,
    uint32_t delay,
    lv_anim_ready_cb_t ready_cb,
    lv_anim_path_cb_t path_cb
)
{
    lv_anim_t anim_temp;

    if (a == NULL)
    {
        a = &anim_temp;

        /* INITIALIZE AN ANIMATION
        *-----------------------*/
        lv_anim_init(a);
    }

    /* MANDATORY SETTINGS
     *------------------*/

    /*Set the "animator" function*/
    lv_anim_set_exec_cb(a, exec_cb);

    /*Set the "animator" function*/
    lv_anim_set_var(a, obj);

    /*Length of the animation [ms]*/
    lv_anim_set_time(a, time);

    /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_values(a, start, end);


    /* OPTIONAL SETTINGS
     *------------------*/

    /*Time to wait before starting the animation [ms]*/
    lv_anim_set_delay(a, delay);

    /*Set the path in an animation*/
    lv_anim_set_path_cb(a, path_cb);

    /*Set a callback to call when animation is ready.*/
    lv_anim_set_ready_cb(a, ready_cb);

    /*Set a callback to call when animation is started (after delay).*/
    lv_anim_set_start_cb(a, ready_cb);

    /* START THE ANIMATION
     *------------------*/
    lv_anim_start(a);                             /*Start the animation*/
}