//
// Created by Coward on 2024/12/20.
//

#ifndef HAL_APPLICATION_H
#define HAL_APPLICATION_H
#include "lvgl.h"
void MusicPlayer(void);
#define scr_get_width() lv_obj_get_width(lv_scr_act())
#define scr_get_height() lv_obj_get_height(lv_scr_act())

typedef union TMyMusicPlayer
{
    struct _MusicPlayerConfig
    {
        uint8_t SliderValue;        //
        uint8_t SliderChangedValue; //
        uint8_t ValueIncrement : 1; //
        uint8_t ValueDecrement : 1; //
        uint8_t VolumeChange : 1;   //
        uint8_t LightChange : 1;    //
        uint8_t PlayBtnClicked : 1;
        uint8_t PauseBtnClicked : 1;
        uint8_t StopBtnClicked : 1;
        uint8_t PreviousTrackBtnClicked : 1;
        uint8_t NextTrackBtnClicked : 1;
        uint8_t MuteBtnClicked : 1;
        uint8_t BtnClicked : 1;
        uint8_t reserved : 5; //
    } MusicPlayerConfig;
    uint32_t MusicPlayerValue; // 整体滑块值
} MyMusicPlayer;

#endif // HAL_APPLICATION_H
