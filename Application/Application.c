#include "Application.h"
#include "lvgl.h"
#include "FreeRTOS.h"
#include "task.h"

// 定义最大视图数量

static lv_obj_t *MenuView;

// 声明菜单按钮和滑块对象
lv_obj_t *Play_Btn;
lv_obj_t *PreviousTrack_Btn;
lv_obj_t *NextTrack_Btn;
lv_obj_t *Mute_Btn;
lv_obj_t *Stop_Btn;
lv_obj_t *LIGHT_Slider;
lv_obj_t *VOLUMN_Slider;
lv_obj_t *VOLUMN_Val;
lv_obj_t *LIGHT_Val;
lv_obj_t *LIGHT_LB;
lv_obj_t *VOLUMN_LB;

static lv_obj_t *PageOne;
uint8_t SliderVolVal = 0;
uint8_t SliderLightVal = 0;
// 声明样式对象
lv_style_t FontStyle;

// 滑块事件回调函数
extern MyMusicPlayer mMusicPlayer;
extern TaskHandle_t MusicPlayerTask_Handler;
static void SliderEvent_Cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *tgt = (lv_obj_t *)lv_event_get_target(e);
    static uint8_t VolVal = 0;
    static uint8_t LightVal = 0;
    static uint8_t LastVolVal = 0;
    static uint8_t LastLightVal = 0;
    uint8_t SliderVolRef = 0;
    uint8_t SliderLightRef = 0;
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        mMusicPlayer.MusicPlayerValue = 0;
        if (tgt == LIGHT_Slider)
        {
            LightVal = lv_slider_get_value(LIGHT_Slider);
            if (LastLightVal > LightVal)
            {
                SliderLightRef = LastLightVal - LightVal;
                mMusicPlayer.MusicPlayerConfig.ValueDecrement = 1;
            }
            else
            {
                SliderLightRef = LightVal - LastLightVal;
                mMusicPlayer.MusicPlayerConfig.ValueIncrement = 1;
            }
            LastLightVal = LightVal;
            SliderLightVal = LightVal;

            mMusicPlayer.MusicPlayerConfig.LightChange = 1;
            mMusicPlayer.MusicPlayerConfig.SliderValue = LightVal;
            mMusicPlayer.MusicPlayerConfig.SliderChangedValue = SliderLightRef;
            xTaskNotify(MusicPlayerTask_Handler, mMusicPlayer.MusicPlayerValue, eSetValueWithOverwrite);
            lv_label_set_text_fmt(LIGHT_Val, "%d%%", LightVal);
        }
        else
        {
            VolVal = lv_slider_get_value(VOLUMN_Slider);

            if (LastVolVal > VolVal)
            {
                SliderVolRef = LastVolVal - VolVal;
                mMusicPlayer.MusicPlayerConfig.ValueDecrement = 1;
            }
            else
            {
                SliderVolRef = VolVal - LastVolVal;
                mMusicPlayer.MusicPlayerConfig.ValueIncrement = 1;
            }
            LastVolVal = VolVal;
            SliderVolVal = VolVal;
            mMusicPlayer.MusicPlayerConfig.VolumeChange = 1;
            mMusicPlayer.MusicPlayerConfig.SliderValue = VolVal;
            mMusicPlayer.MusicPlayerConfig.SliderChangedValue = SliderVolRef;
            xTaskNotify(MusicPlayerTask_Handler, mMusicPlayer.MusicPlayerValue, eSetValueWithOverwrite);
            lv_label_set_text_fmt(VOLUMN_Val, "%d%%", VolVal);
        }
    }
}
void BtnsEvent_Cb(lv_event_t *e)
{
    lv_obj_t *tgt = (lv_obj_t *)lv_event_get_target(e);
    static bool PLAYBTNCLICKED = 0;
    mMusicPlayer.MusicPlayerConfig.BtnClicked = 1;
    if (tgt == Play_Btn)
    {
        if (!PLAYBTNCLICKED)
        {
            mMusicPlayer.MusicPlayerConfig.PlayBtnClicked = 1;
            PLAYBTNCLICKED = !PLAYBTNCLICKED;
            lv_obj_set_style_bg_img_src(Play_Btn, LV_SYMBOL_PAUSE, LV_STATE_DEFAULT);
        }
        else
        {
            mMusicPlayer.MusicPlayerConfig.PauseBtnClicked = 1;
            PLAYBTNCLICKED = !PLAYBTNCLICKED;
            lv_obj_set_style_bg_img_src(Play_Btn, LV_SYMBOL_PLAY, LV_STATE_DEFAULT);
        }
    }
    else if (tgt == PreviousTrack_Btn)
    {
        mMusicPlayer.MusicPlayerConfig.PreviousTrackBtnClicked = 1;
    }
    else if (tgt == NextTrack_Btn)
    {
        mMusicPlayer.MusicPlayerConfig.NextTrackBtnClicked = 1;
    }
    else if (tgt == Mute_Btn)
    {
        mMusicPlayer.MusicPlayerConfig.MuteBtnClicked = 1;
    }
    else if (tgt == Stop_Btn)
    {
        mMusicPlayer.MusicPlayerConfig.StopBtnClicked = 1;
    }
    xTaskNotify(MusicPlayerTask_Handler, mMusicPlayer.MusicPlayerValue, eSetValueWithOverwrite);
}

// 自动分配菜单按钮和滑块
void SetupPageOneUI(lv_obj_t *menuview)
{
    MenuView = lv_tileview_add_tile(menuview, 0, 0, LV_DIR_BOTTOM);
    Play_Btn = lv_btn_create(MenuView);
    PreviousTrack_Btn = lv_btn_create(MenuView);
    NextTrack_Btn = lv_btn_create(MenuView);
    Mute_Btn = lv_btn_create(MenuView);
    Stop_Btn = lv_btn_create(MenuView);

    LIGHT_Slider = lv_slider_create(MenuView);
    VOLUMN_Slider = lv_slider_create(MenuView);
    LIGHT_Val = lv_label_create(LIGHT_Slider);
    VOLUMN_Val = lv_label_create(VOLUMN_Slider);
    LIGHT_LB = lv_label_create(MenuView);
    VOLUMN_LB = lv_label_create(MenuView);
    // 初始化字体样式
    lv_style_init(&FontStyle);
    lv_style_set_text_color(&FontStyle, lv_color_hex(0xFFFFFF)); // 设置文本颜色为白色
    lv_style_set_bg_opa(&FontStyle, 0);                          // 设置背景透明
    lv_style_set_border_width(&FontStyle, 0);                    // 设置边框宽度为0

    // 设置标签文本
    lv_label_set_text(LIGHT_LB, "Light");
    lv_label_set_text(VOLUMN_LB, "Volumn");
    lv_label_set_text(LIGHT_Val, "0%");
    lv_label_set_text(VOLUMN_Val, "0%");

    // 应用字体样式
    lv_obj_add_style(LIGHT_LB, &FontStyle, LV_STATE_DEFAULT);
    lv_obj_add_style(VOLUMN_LB, &FontStyle, LV_STATE_DEFAULT);
    lv_obj_add_style(VOLUMN_Val, &FontStyle, LV_STATE_DEFAULT);
    lv_obj_add_style(LIGHT_Val, &FontStyle, LV_STATE_DEFAULT);

    // 设置滑块范围
    lv_slider_set_range(LIGHT_Slider, 0, 100);
    lv_slider_set_range(VOLUMN_Slider, 0, 100);

    // 添加滑块事件回调
    lv_obj_add_event_cb(LIGHT_Slider, SliderEvent_Cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(VOLUMN_Slider, SliderEvent_Cb, LV_EVENT_VALUE_CHANGED, NULL);
    uint8_t padding = 5;
    lv_obj_set_size(LIGHT_Slider, scr_get_width() / 4, scr_get_height() * 3 / 4 - padding);
    lv_obj_set_size(VOLUMN_Slider, scr_get_width() / 4, scr_get_height() * 3 / 4 - padding);
    lv_obj_set_size(Play_Btn, scr_get_width() / 5 - padding, scr_get_height() * 1 / 4 - padding);
    lv_obj_set_size(Mute_Btn, scr_get_width() / 5 - padding, scr_get_height() * 1 / 4 - padding);
    lv_obj_set_size(NextTrack_Btn, scr_get_width() / 5 - padding, scr_get_height() * 1 / 4 - padding);
    lv_obj_set_size(PreviousTrack_Btn, scr_get_width() / 5 - padding, scr_get_height() * 1 / 4 - padding);
    lv_obj_set_size(Stop_Btn, scr_get_width() / 5 - padding, scr_get_height() * 1 / 4 - padding);
    // 设置按钮和滑块的位置和大小
    int btn_gap = scr_get_width() / 5;
    // Play Button
    lv_obj_align(Play_Btn, LV_ALIGN_TOP_MID, 0, 0 + padding);

    // Mute_Btn Button
    lv_obj_align(PreviousTrack_Btn, LV_ALIGN_TOP_MID, -btn_gap, 0 + padding);

    // NextTrack Button
    lv_obj_align(NextTrack_Btn, LV_ALIGN_TOP_MID, btn_gap, 0 + padding);

    // PreviousTrack_Btn Button
    lv_obj_align(Stop_Btn, LV_ALIGN_TOP_MID, -2 * btn_gap, 0 + padding);
    // Rewind Button
    lv_obj_align(Mute_Btn, LV_ALIGN_TOP_MID, 2 * btn_gap, 0 + padding);

    lv_obj_align(LIGHT_Slider, LV_ALIGN_TOP_MID, -scr_get_width() / 4,
                 scr_get_height() / 4 + padding);
    lv_obj_align(VOLUMN_Slider, LV_ALIGN_TOP_MID, scr_get_width() / 4,
                 scr_get_height() / 4 + padding);

    // 设置按钮样式
    lv_obj_set_style_bg_color(Play_Btn, lv_color_hex(0x565656), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(PreviousTrack_Btn, lv_color_hex(0x565656), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(NextTrack_Btn, lv_color_hex(0x565656), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(Mute_Btn, lv_color_hex(0x565656), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(Stop_Btn, lv_color_hex(0x565656), LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(PreviousTrack_Btn, lv_color_hex(0xFF0000), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(NextTrack_Btn, lv_color_hex(0xFF0000), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(Mute_Btn, lv_color_hex(0xFF0000), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(Stop_Btn, lv_color_hex(0xFF0000), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(Play_Btn, lv_color_hex(0xFF0000), LV_STATE_PRESSED);

    lv_obj_set_style_bg_opa(Play_Btn, 150, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(PreviousTrack_Btn, 150, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(NextTrack_Btn, 150, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Mute_Btn, 150, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Stop_Btn, 150, LV_STATE_DEFAULT);

    lv_obj_set_style_shadow_width(Play_Btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(PreviousTrack_Btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(NextTrack_Btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(Mute_Btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(Stop_Btn, 0, LV_STATE_DEFAULT);

    lv_obj_set_style_bg_img_src(Play_Btn, LV_SYMBOL_PLAY, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(PreviousTrack_Btn, LV_SYMBOL_PREV, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(NextTrack_Btn, LV_SYMBOL_NEXT, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(Mute_Btn, LV_SYMBOL_MUTE, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(Stop_Btn, LV_SYMBOL_STOP, LV_STATE_DEFAULT);

    // 设置滑块样式
    lv_obj_set_style_bg_color(LIGHT_Slider, lv_color_hex(0xfafafa),
                              LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(LIGHT_Slider, lv_color_hex(0x565656), LV_PART_MAIN);
    lv_obj_set_style_bg_color(VOLUMN_Slider, lv_color_hex(0xfafafa),
                              LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(VOLUMN_Slider, lv_color_hex(0x565656),
                              LV_PART_MAIN);

    // 设置标签位置
    lv_obj_set_style_text_color(LIGHT_Val, lv_color_hex(0x000000),
                                LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(VOLUMN_Val, lv_color_hex(0x000000),
                                LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(LIGHT_LB, lv_color_hex(0x000000),
                                LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(VOLUMN_LB, lv_color_hex(0x000000),
                                LV_STATE_DEFAULT);

    lv_obj_align_to(LIGHT_Val, LIGHT_Slider, LV_ALIGN_CENTER, -5, 20);
    lv_obj_align_to(VOLUMN_Val, VOLUMN_Slider, LV_ALIGN_CENTER, -5, 20);
    lv_obj_align_to(LIGHT_LB, LIGHT_Slider, LV_ALIGN_CENTER, 0, -20);
    lv_obj_align_to(VOLUMN_LB, VOLUMN_Slider, LV_ALIGN_CENTER, 0, -20);

    // 移除滑块的旋钮样式
    lv_obj_remove_style(LIGHT_Slider, NULL, LV_PART_KNOB);
    lv_obj_remove_style(VOLUMN_Slider, NULL, LV_PART_KNOB);
    // 事件
    lv_obj_add_event_cb(Play_Btn, BtnsEvent_Cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(PreviousTrack_Btn, BtnsEvent_Cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(NextTrack_Btn, BtnsEvent_Cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(Mute_Btn, BtnsEvent_Cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(Stop_Btn, BtnsEvent_Cb, LV_EVENT_CLICKED, NULL);
}
void MusicPlayer(void)
{
    PageOne = lv_tileview_create(lv_scr_act());
    lv_obj_set_style_bg_color(PageOne, lv_color_hex(0x1e1e1e), LV_PART_MAIN);
    SetupPageOneUI(PageOne);
}
