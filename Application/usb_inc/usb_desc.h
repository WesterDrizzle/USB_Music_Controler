/**
 ******************************************************************************
 * @file    usb_desc.h
 * @author  MCD Application Team
 * @version V4.1.0
 * @date    26-May-2017
 * @brief   Descriptor Header for MusicPlayer Mouse Demo
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE 0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE 0x02
#define USB_STRING_DESCRIPTOR_TYPE 0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE 0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE 0x05

#define HID_DESCRIPTOR_TYPE 0x21
#define MUSICPLAYER_SIZ_HID_DESC 0x09
#define MUSICPLAYER_OFF_HID_DESC 0x12

#define MUSICPLAYER_SIZ_DEVICE_DESC 18
#define MUSICPLAYER_SIZ_CONFIG_DESC 34
#define MUSICPLAYER_SIZ_REPORT_DESC 51 // 29//74
#define MUSICPLAYER_SIZ_STRING_LANGID 4
#define MUSICPLAYER_SIZ_STRING_VENDOR 38
#define MUSICPLAYER_SIZ_STRING_PRODUCT 58 // 30
#define MUSICPLAYER_SIZ_STRING_SERIAL 26

#define STANDARD_ENDPOINT_DESC_SIZE 0x09

/* Exported functions ------------------------------------------------------- */
extern const uint8_t MusicPlayer_DeviceDescriptor[MUSICPLAYER_SIZ_DEVICE_DESC];
extern const uint8_t MusicPlayer_ConfigDescriptor[MUSICPLAYER_SIZ_CONFIG_DESC];
extern const uint8_t MusicPlayer_ReportDescriptor[MUSICPLAYER_SIZ_REPORT_DESC];
extern const uint8_t MusicPlayer_StringLangID[MUSICPLAYER_SIZ_STRING_LANGID];
extern const uint8_t MusicPlayer_StringVendor[MUSICPLAYER_SIZ_STRING_VENDOR];
extern const uint8_t MusicPlayer_StringProduct[MUSICPLAYER_SIZ_STRING_PRODUCT];
extern uint8_t MusicPlayer_StringSerial[MUSICPLAYER_SIZ_STRING_SERIAL];

#endif /* __USB_DESC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
