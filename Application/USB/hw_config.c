/**
 ******************************************************************************
 * @file    hw_config.c
 * @author  MCD Application Team
 * @version V4.1.0
 * @date    26-May-2017
 * @brief   Hardware Configuration & Setup
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
#include "hw_config.h"
#include "tftlcd.h"
#include "stdio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len);
static void Delay(__IO uint32_t nTime);
/* Private functions ---------------------------------------------------------*/
GPIO_TypeDef *BUTTON_PORT[4] = {
    GPIOA, // up
    GPIOE, // down
    GPIOE, // left
    GPIOE, // right
};
uint16_t BUTTON_PIN[4] = {
    GPIO_Pin_0,
    GPIO_Pin_3, // down
    GPIO_Pin_2, // left
    GPIO_Pin_4};
uint32_t BUTTON_CLK[4] = {
    RCC_APB2Periph_GPIOA,
    RCC_APB2Periph_GPIOE,
    RCC_APB2Periph_GPIOE,
    RCC_APB2Periph_GPIOE,
};

uint32_t STM_PBGetState(Button_TypeDef BUTTON)
{
    if (BUTTON == BUTTON_UP)
    {
        return GPIO_ReadInputDataBit(BUTTON_PORT[BUTTON], BUTTON_PIN[BUTTON]);
    }
    else
    {
        return !GPIO_ReadInputDataBit(BUTTON_PORT[BUTTON], BUTTON_PIN[BUTTON]);
    }
}
void STM_PBInit(Button_TypeDef BUTTON)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable the BUTTON Clock */
    RCC_APB2PeriphClockCmd(BUTTON_CLK[BUTTON] | RCC_APB2Periph_AFIO, ENABLE);
    /* Configure BUTTON pin as input floating */
    if (BUTTON == BUTTON_UP)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    }
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[BUTTON];
    GPIO_Init(BUTTON_PORT[BUTTON], &GPIO_InitStructure);
}
/**
 * Function Name  : Set_System
 * Description    : Configures Main system clocks & power.
 * Input          : None.
 * Return         : None.
 */

void Set_System(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*!< At this stage the microcontroller clock setting is already configured,
         this is done through SystemInit() function which is called from startup
         file (startup_stm32xxx.s) before to branch to application main.
         To reconfigure the default setting of SystemInit() function, refer to
         system_stm32xxx.c file
       */

    /******************************************/
    /*           Enable the PWR clock         */
    /******************************************/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /********************************************/
    /*  Configure USB DM/DP pins                */
    /********************************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12; // USB
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Enable all GPIOs Clock*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);

    /****************************************************/
    /*  Configure the MusicPlayer buttons in GPIO mode     */
    /****************************************************/

    STM_PBInit(BUTTON_RIGHT);
    STM_PBInit(BUTTON_LEFT);
    STM_PBInit(BUTTON_UP);
    STM_PBInit(BUTTON_DOWN);
}

/**
 * Function Name  : Set_USBClock
 * Description    : Configures USB Clock input (48MHz).
 * Input          : None.
 * Output         : None.
 * Return         : None.
 */
void Set_USBClock(void)
{

    /* Select USBCLK source */
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
    /* Enable the USB clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/**
 * Function Name  : Leave_LowPowerMode.
 * Description    : Restores system clocks and power while exiting suspend mode.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 */
void Leave_LowPowerMode(void)
{
    DEVICE_INFO *pInfo = &Device_Info;

    /* Set the device state to the correct state */
    if (pInfo->Current_Configuration != 0)
    {
        /* Device configured */
        bDeviceState = CONFIGURED;
    }
    else
    {
        bDeviceState = ATTACHED;
    }
}

/**
 * Function Name  : USB_Interrupts_Config.
 * Description    : Configures the USB interrupts.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 */
void USB_Interrupts_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 2 bit for pre-emption priority, 2 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /* Enable the USB interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * Function Name  : USB_Cable_Config.
 * Description    : Software Connection/Disconnection of USB Cable.
 * Input          : NewState: new state.
 * Output         : None.
 * Return         : None
 */
void USB_Cable_Config(FunctionalState NewState)
{

    if (NewState != DISABLE)
    {
    }
    else
    {
    }
}
/**
 * Function Name : JoyState.
 * Description   : Decodes the MusicPlayer direction.
 * Input         : None.
 * Output        : None.
 * Return value  : The direction value.
 */
signed char BtnState(void)
{
    /* "right" key is pressed */

    if (STM_PBGetState(BUTTON_RIGHT))
    {
        return RIGHT;
    }
    /* "left" key is pressed */

    if (STM_PBGetState(BUTTON_LEFT))
    {
        return LEFT;
    }
    /* "up" key is pressed */
    if (STM_PBGetState(BUTTON_UP))
    {
        return UP;
    }
    /* "down" key is pressed */

    if (STM_PBGetState(BUTTON_DOWN))
    {
        return DOWN;
    }
    /* No key is pressed */
    else
    {
        return 0;
    }
}

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
static void Delay(__IO uint32_t nTime)
{
    for (; nTime != 0; nTime--)
        ;
}

/**
 * Function Name  : Get_SerialNum.
 * Description    : Create the serial number string descriptor.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 */
void Get_SerialNum(void)
{
    uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

    Device_Serial0 = *(uint32_t *)ID1;
    Device_Serial1 = *(uint32_t *)ID2;
    Device_Serial2 = *(uint32_t *)ID3;

    Device_Serial0 += Device_Serial2;

    if (Device_Serial0 != 0)
    {
        IntToUnicode(Device_Serial0, &MusicPlayer_StringSerial[2], 8); // unicode 2+2*8 = 18
        IntToUnicode(Device_Serial1, &MusicPlayer_StringSerial[18], 4);
    }
}

/**
 * Function Name  : HexToChar.
 * Description    : Convert Hex 32Bits value into char.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 */
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len)
{
    uint8_t idx = 0;

    for (idx = 0; idx < len; idx++)
    {
        if (((value >> 28)) < 0xA)               // 4位
        {                                        // unicode :两个字节对应一个字符
            pbuf[2 * idx] = (value >> 28) + '0'; // hex对应的字符串
        }
        else
        {
            pbuf[2 * idx] = (value >> 28) + 'A' - 10; // 注意是HEX转换0-F
        }

        value = value << 4;

        pbuf[2 * idx + 1] = 0;
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
