/**
 ******************************************************************************
 * @file    usb_desc.c
 * @author  MCD Application Team
 * @version V4.1.0
 * @date    26-May-2017
 * @brief   Descriptors for MusicPlayer Mouse Demo
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

/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t MusicPlayer_DeviceDescriptor[MUSICPLAYER_SIZ_DEVICE_DESC] =
    {
        0x12,                       /*bLength */
        USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
        0x00,                       /*bcdUSB */
        0x02,
        0x00, /*bDeviceClass*/
        0x00, /*bDeviceSubClass*/
        0x00, /*bDeviceProtocol*/
        0x40, /*bMaxPacketSize 64*/
        0x83, /*idVendor (0x0483)*/
        0x04,
        0x10, /*idProduct = 0x5710*/
        0x57,
        0x00, /*bcdDevice rel. 2.00*/
        0x02,
        1,   /*Index of string descriptor describing
                               manufacturer */
        2,   /*Index of string descriptor describing
                              product*/
        3,   /*Index of string descriptor describing the
                              device serial number */
        0x01 /*bNumConfigurations*/
}; /* MusicPlayer_DeviceDescriptor */

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t MusicPlayer_ConfigDescriptor[MUSICPLAYER_SIZ_CONFIG_DESC] =
    {
        0x09,                              /* bLength: Configuration Descriptor size */
        USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
        MUSICPLAYER_SIZ_CONFIG_DESC,
        /* wTotalLength: Bytes returned */
        0x00,
        0x01, /*bNumInterfaces: 1 interface*/
        0x01, /*bConfigurationValue: Configuration value*/
        0x00, /*iConfiguration: Index of string descriptor describing
                                 the configuration*/
        0xE0, /*bmAttributes: Self powered */
        0x32, /*MaxPower 100 mA: this current is used for detecting Vbus*/

        /************** Descriptor of MusicPlayer Mouse interface ****************/
        /* 09 */
        0x09,                          /*bLength: Interface Descriptor size*/
        USB_INTERFACE_DESCRIPTOR_TYPE, /*bDescriptorType: Interface descriptor type*/
        0x00,                          /*bInterfaceNumber: Number of Interface*/
        0x00,                          /*bAlternateSetting: Alternate setting*/
        0x01,                          /*bNumEndpoints*/
        0x03,                          /*bInterfaceClass: HID*/
        0x00,                          /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
        0x00,                          /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
        0,                             /*iInterface: Index of string descriptor*/
        /******************** Descriptor of MusicPlayer Mouse HID ********************/
        /* 18 */
        0x09,                /*bLength: HID Descriptor size*/
        HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
        0x00,                /*bcdHID: HID Class Spec release number*/
        0x01,
        0x00,                        /*bCountryCode: Hardware target country*/
        0x01,                        /*bNumDescriptors: Number of HID class descriptors to follow*/
        0x22,                        /*bDescriptorType*/
        MUSICPLAYER_SIZ_REPORT_DESC, /*wItemLength: Total length of Report descriptor*/
        0x00,
        /******************** Descriptor of MusicPlayer Mouse endpoint ********************/
        /* 27 */
        0x07,                         /*bLength: Endpoint Descriptor size*/
        USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

        0x81, /*bEndpointAddress: Endpoint Address (IN)*/
        0x03, /*bmAttributes: Interrupt endpoint*/
        0x04, /*wMaxPacketSize: 4 Byte max */
        0x00,
        0x20, /*bInterval: Polling Interval (32 ms)*/
              /* 34 */
};
const uint8_t MusicPlayer_ReportDescriptor[MUSICPLAYER_SIZ_REPORT_DESC] = {

    0x05, 0x0c, // USAGE_PAGE (Consumer Devices)
    0x09, 0x01, // USAGE (Consumer Control)
    0xa1, 0x01, // COLLECTION (Application)
    /*播放器*/  // Byte1
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x01, //   LOGICAL_MAXIMUM (1)
    0x75, 0x01, //   REPORT_SIZE (1)
    0x95, 0x08, //   REPORT_COUNT (8)
    0x09, 0xb6, //   USAGE (Scan Previous Track)
    0x09, 0xb5, //   USAGE (Scan Next Track)
    0x09, 0xb0, //   USAGE (Play)
    0x09, 0xb1, //   USAGE (Pause)
    0x09, 0xe9, //   USAGE (Volume Up)
    0x09, 0xea, //   USAGE (Volume Down)
    0x09, 0xE2, //   USAGE (Mute)
    0x09, 0xb7, //   USAGE (Stop)
    0x81, 0x02, //   INPUT (Data,Var,Abs)
    /*亮度*/
    // Byte2
    0x75, 0x01, //   REPORT_SIZE (1)
    0x95, 0x02, //   REPORT_COUNT (2)
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x01, //   LOGICAL_MAXIMUM (1)
    0x09, 0x6F, //   USAGE (Light Inc)
    0x09, 0x70, //   USAGE (Light Dec)
    0x81, 0x02, //   INPUT (Data,Var,Abs)
    0x95, 0x06, //   REPORT_COUNT (6)
    0x81, 0x01, //   INPUT (Cnst,Var,Abs)
    0xc0        //   EndCOLLECTION (Application)

};

/* MOUSE_ConfigDescriptor */

// const uint8_t MusicPlayer_ReportDescriptor[MUSICPLAYER_SIZ_REPORT_DESC] =
//     {
//         0x05, 0x01, /*Usage Page(Generic Desktop)*/
//         0x09, 0x02, /*Usage(Mouse)*/
//         0xA1, 0x01, /*Collection(Logical)*/
//         0x09, 0x01, /*Usage(Pointer)*/
//         /* 8 */
//         0xA1, 0x00, /*Collection(Linked)*/
//         0x05, 0x09, /*Usage Page(Buttons)*/
//         0x19, 0x01, /*Usage Minimum(1)*/
//         0x29, 0x03, /*Usage Maximum(3)*/
//         /* 16 */
//         0x15, 0x00, /*Logical Minimum(0)*/
//         0x25, 0x01, /*Logical Maximum(1)*/
//         0x95, 0x03, /*Report Count(3)*/
//         0x75, 0x01, /*Report Size(1)*/
//         /* 24 */
//         0x81, 0x02, /*Input(Variable)*/
//         0x95, 0x01, /*Report Count(1)*/
//         0x75, 0x05, /*Report Size(5)*/
//         0x81, 0x01, /*Input(Constant,Array)*/
//         /* 32 */
//         0x05, 0x01, /*Usage Page(Generic Desktop)*/
//         0x09, 0x30, /*Usage(X axis)*/
//         0x09, 0x31, /*Usage(Y axis)*/
//         0x09, 0x38, /*Usage(Wheel)*/
//         /* 40 */
//         0x15, 0x81, /*Logical Minimum(-127)*/
//         0x25, 0x7F, /*Logical Maximum(127)*/
//         0x75, 0x08, /*Report Size(8)*/
//         0x95, 0x03, /*Report Count(3)*/
//         /* 48 */
//         0x81, 0x06, /*Input(Variable, Relative)*/
//         0xC0,       /*End Collection*/
//         0x09,
//         0x3c,
//         0x05,
//         0xff,
//         0x09,
//         /* 56 */
//         0x01,
//         0x15,
//         0x00,
//         0x25,
//         0x01,
//         0x75,
//         0x01,
//         0x95,
//         /* 64 */
//         0x02,
//         0xb1,
//         0x22,
//         0x75,
//         0x06,
//         0x95,
//         0x01,
//         0xb1,
//         /* 72 */
//         0x01,
//         0xc0}; /* MusicPlayer_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t MusicPlayer_StringLangID[MUSICPLAYER_SIZ_STRING_LANGID] =
    {
        MUSICPLAYER_SIZ_STRING_LANGID,
        USB_STRING_DESCRIPTOR_TYPE,
        0x09,
        0x04}; /* LangID = 0x0409: U.S. English */

const uint8_t MusicPlayer_StringVendor[MUSICPLAYER_SIZ_STRING_VENDOR] =
    {
        MUSICPLAYER_SIZ_STRING_VENDOR, /* Size of Vendor string */
        USB_STRING_DESCRIPTOR_TYPE,    /* bDescriptorType*/
        /* Manufacturer: "STMicroelectronics" */
        'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
        'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
        'c', 0, 's', 0};
const uint8_t MusicPlayer_StringProduct[MUSICPLAYER_SIZ_STRING_PRODUCT] =
    {
        MUSICPLAYER_SIZ_STRING_PRODUCT, /* bLength */
        USB_STRING_DESCRIPTOR_TYPE,     /* bDescriptorType */
        'M', 0, 'u', 0, 's', 0, 'i', 0, 'c', 0, ' ', 0, 'P', 0,
        'l', 0, 'a', 0, 'y', 0, 'e', 0, 'r', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0, 't', 0, 'r', 0, 'o', 0, 'l', 0, 'l', 0, 'e', 0, 'r', 0, ' ', 0, 'V', 0, '1', 0, '.', 0, '0', 0};
uint8_t MusicPlayer_StringSerial[MUSICPLAYER_SIZ_STRING_SERIAL] =
    {
        MUSICPLAYER_SIZ_STRING_SERIAL, /* bLength */
        USB_STRING_DESCRIPTOR_TYPE,    /* bDescriptorType */
        'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0};

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
