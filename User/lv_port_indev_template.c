static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static TickType_t last_touch_time = 0;
    TickType_t current_time = xTaskGetTickCount();

    // 读取触摸数据
    portENTER_CRITICAL();
    tp_dev.scan(0);
    portEXIT_CRITICAL();

    if (tp_dev.sta & TP_PRES_DOWN) // 触摸按下
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = tp_dev.x[0];
        data->point.y = tp_dev.y[0];
        last_touch_time = current_time;
    }
    else
    {
        // 添加10ms的消抖时间
        if ((current_time - last_touch_time) > pdMS_TO_TICKS(10))
        {
            data->state = LV_INDEV_STATE_REL;
        }
    }
}