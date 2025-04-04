#include "displayPrivate.h"

static const char *TAG = "lvgl_port";
/*** Setup screen resolution for LVGL ***/
static const uint16_t screenWidth = TFT_WIDTH;
static const uint16_t screenHeight = TFT_HEIGHT;

#define DISP_BUF_SIZE        (TFT_WIDTH * TFT_HEIGHT / 2)
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
static lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);

static lv_disp_t *disp;
// static lv_theme_t *theme_current;
// static lv_color_t bg_theme_color;

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
static SemaphoreHandle_t xGuiSemaphore = NULL;
static TaskHandle_t g_lvgl_task_handle;

static void gui_task(void *args);

/*** Function declaration ***/
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

#ifdef TOUCH_ENABLED
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
#endif

static void lv_tick_task(void *arg);

esp_err_t lv_display_init()
{
    // Setting display to landscape
    // if (lcd.width() < lcd.height()) lcd.setRotation(lcd.getRotation() ^ 2);
    
    lcd.setColorDepth(16);
    lcd.setRotation(Rotation);
#ifdef RTOUCH
    /* RESISTIVE TOUCH CALIBRATION */
    // Calibrates when touch is available. (Optional)
    if (lcd.touch())
    {
        if (lcd.width() < lcd.height())
            lcd.setRotation(lcd.getRotation() ^ 1);

        // Draw a guide sentence on the screen.
        lcd.setTextDatum(textdatum_t::middle_center);
        lcd.drawString("touch the arrow marker.", lcd.width() >> 1, lcd.height() >> 1);
        lcd.setTextDatum(textdatum_t::top_left);

        // When using touch, calibrate it. Touch the tips of the arrows that appear in the four corners of the screen in order.
        std::uint16_t fg = TFT_WHITE;
        std::uint16_t bg = TFT_BLACK;
        if (lcd.isEPD())
            std::swap(fg, bg);
        lcd.calibrateTouch(nullptr, fg, bg, std::max(lcd.width(), lcd.height()) >> 3);
    }
    /* CALIBRATION */
#endif

    // lcd.fillScreen(TFT_BLACK);

    /* LVGL : Setting up buffer to use for display */
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, DISP_BUF_SIZE * 2);

    /*** LVGL : Setup & Initialize the display device driver ***/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = display_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.sw_rotate = 1;
    disp = lv_disp_drv_register(&disp_drv);

#ifdef TOUCH_ENABLED
    //*** LVGL : Setup & Initialize the input device driver ***
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
#endif

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t lv_periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
        };
    esp_timer_handle_t lv_periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&lv_periodic_timer_args, &lv_periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lv_periodic_timer, LV_TICK_PERIOD_MS * 1000));

    // // Setup theme
    // theme_current = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE),
    //                                       lv_palette_main(LV_PALETTE_RED),
    //                                       LV_USE_THEME_DEFAULT, /*Light or dark mode*/
    //                                       &lv_font_montserrat_14);

    // // lv_disp_set_theme(disp, th); /*Assign the theme to the display*/
    // bg_theme_color = theme_current->flags & LV_USE_THEME_DEFAULT ? lv_palette_darken(LV_PALETTE_GREY, 4) : lv_palette_lighten(LV_PALETTE_GREY, 1);

    xGuiSemaphore = xSemaphoreCreateMutex();
    if (!xGuiSemaphore)  
    {
        ESP_LOGE(TAG, "Create mutex for LVGL failed");
        if (lv_periodic_timer) esp_timer_delete(lv_periodic_timer);
        return ESP_FAIL;
    }

#if CONFIG_FREERTOS_UNICORE == 0
    int err = xTaskCreatePinnedToCore(gui_task, "lv gui", 1024 * 8, NULL, 5, &g_lvgl_task_handle, 1);
#else
    int err = xTaskCreatePinnedToCore(gui_task, "lv gui", 1024 * 8, NULL, 5, &g_lvgl_task_handle, 0);
#endif
    if (!err)
    {
        ESP_LOGE(TAG, "Create task for LVGL failed");
        if (lv_periodic_timer) esp_timer_delete(lv_periodic_timer);
        return ESP_FAIL;
    }

    esp_timer_start_periodic(lv_periodic_timer, LV_TICK_PERIOD_MS * 1000U);
    return ESP_OK;
}

// Display callback to flush the buffer to screen
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    // uint32_t w = (area->x2 - area->x1 + 1);
    // uint32_t h = (area->y2 - area->y1 + 1);

    // lcd.startWrite();
    // lcd.setAddrWindow(area->x1, area->y1, w, h);
    // lcd.pushPixelsDMA((uint16_t *)&color_p->full, w * h, true);
    // lcd.endWrite();

    // lv_disp_flush_ready(disp);
    if (lcd.getStartCount() == 0)
    {   // Processing if not yet started
        lcd.startWrite();
    }
    lcd.pushImageDMA( area->x1
                    , area->y1
                    , area->x2 - area->x1 + 1
                    , area->y2 - area->y1 + 1
                    , (uint16_t *)&color_p->full);
    lv_disp_flush_ready( disp );
}

/* Setting up tick task for lvgl */
static void lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

static void gui_task(void *args)
{
    ESP_LOGI(TAG, "Start to run LVGL");
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            // ui_tick();
            //lv_timer_handler_run_in_period(5); /* run lv_timer_handler() every 5ms */
            xSemaphoreGive(xGuiSemaphore);
        }
    }
}

void lvgl_acquire(void)
{
    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    if (g_lvgl_task_handle != task) {
        xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
    }
}

void lvgl_release(void)
{
    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    if (g_lvgl_task_handle != task) {
        xSemaphoreGive(xGuiSemaphore);
    }
}