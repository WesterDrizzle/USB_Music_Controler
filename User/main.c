#include "system.h"
#include "SysTick.h"
#include "malloc.h"
#include "usart.h"
#include "timer.h"
#include "sram.h"
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "touch.h"
#include "Application.h"
#include "hw_config.h"

#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

#include "FreeRTOS.h"
#include "task.h"
#include "mutex.h"
#include "semphr.h"
QueueHandle_t lvgl_mutex;
//////////////////////////////////////////任务优先级堆栈分配//////////////////////////////////////////
#define LvglUpdateTASK_PRIO 3
#define LvglUpdateTASK_STACK_SIZE 1024

#define MusicPlayerTASK_PRIO 1
#define MusicPlayerTASK_STACK_SIZE 1024

#define DetectTASK_PRIO 2
#define DetectTASK_STACK_SIZE 512

#define START_TASK_STACK_SIZE 256
#define START_TASK_PRIO 1

//////////////////////////////////////////任务句柄//////////////////////////////////////////
TaskHandle_t MusicPlayerTask_Handler;
TaskHandle_t LvglUpdateTask_Handler;
TaskHandle_t DetectTask_Handler;
TaskHandle_t Start_Task_Handle;
//////////////////////////////////////////任务明//////////////////////////////////////////
void MusicPlayer_task(void *pvParameters);
void Lvgl_update_task(void *pvParameters);
void Detect_task(void *pvParameters);
void start_task(void *pvParameters);

__IO uint8_t PrevXferComplete = 1;
bool pressed = 0;

// 注意IO资源的竞争与访问
// 时钟的优先级不能太高
// FreeRTOS的堆栈分布在FreeRTOS MemoriesPool里不是启动文件定义的主堆栈里
// 原来lvgl使用Systick延时每次读完会关闭掉sysTick造成错误
int main()
{
	lvgl_mutex = xSemaphoreCreateMutex();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SysTick_Init(72);
	USART2_Init(9600);
	Set_System();
	Set_USBClock();
	TP_Init();
	TFTLCD_Init();
	FSMC_SRAM_Init();
	my_mem_init(SRAMIN); // lvgl显存
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	USB_Init();
	USB_Interrupts_Config();
	MusicPlayer();
	// 创建开始任务
	xTaskCreate(
		(TaskFunction_t)start_task,
		(char *)"start_task",
		(uint16_t)START_TASK_STACK_SIZE,
		(void *)NULL,
		(UBaseType_t)START_TASK_PRIO,
		(TaskHandle_t *)&Start_Task_Handle);
	vTaskStartScheduler();

	while (1)
	{
		assert_param(0);
	}
}
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();

	xTaskCreate((TaskFunction_t)Lvgl_update_task,		  // 任务函数
				(const char *)"lvglUpdate",				  // 任务名称
				(uint16_t)LvglUpdateTASK_STACK_SIZE,	  // 任务堆栈大小
				(void *)NULL,							  // 传递给任务函数的参数
				(UBaseType_t)LvglUpdateTASK_PRIO,		  // 任务优先级
				(TaskHandle_t *)&LvglUpdateTask_Handler); // 任务句柄

	xTaskCreate((TaskFunction_t)MusicPlayer_task,		   // 任务函数
				(const char *)"MusicPlayer",			   // 任务名称
				(uint16_t)MusicPlayerTASK_STACK_SIZE,	   // 任务堆栈大小
				(void *)NULL,							   // 传递给任务函数的参数
				(UBaseType_t)MusicPlayerTASK_PRIO,		   // 任务优先级
				(TaskHandle_t *)&MusicPlayerTask_Handler); // 任务句柄
	xTaskCreate((TaskFunction_t)Detect_task,			   // 任务函数
				(const char *)"Detect",					   // 任务名称
				(uint16_t)DetectTASK_STACK_SIZE,		   // 任务堆栈大小
				(void *)NULL,							   // 传递给任务函数的参数
				(UBaseType_t)DetectTASK_PRIO,			   // 任务优先级
				(TaskHandle_t *)&DetectTask_Handler);	   // 任务句柄

	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
};
extern uint8_t SliderVolRef;
extern uint8_t SliderLightRef;
extern bool SliderVolInc;
extern bool SliderLightInc;
QueueHandle_t USBTransferCompleted;
MyMusicPlayer mMusicPlayer = {0};
void MusicPlayer_task(void *pvParameters)
{
	// BaseType_t xHigherPriorityTaskWoken;
	my_player Player = {0};
	uint32_t Player_Buffer = 0;
	u16 remain;
	USBTransferCompleted = xSemaphoreCreateBinary();
	xSemaphoreGive(USBTransferCompleted);

	while (1)
	{

		if (xTaskNotifyWait(0, 0xFFFFFFF, &mMusicPlayer.MusicPlayerValue, portMAX_DELAY) == pdTRUE)
		{
			if (mMusicPlayer.MusicPlayerConfig.VolumeChange)
			{
				if (mMusicPlayer.MusicPlayerConfig.ValueIncrement)
				{
					Player.btns.VolumeUp = 1;
					remain = mMusicPlayer.MusicPlayerConfig.SliderChangedValue;
					remain = (remain * 50) / 100;
					while (remain--)
					{
						xSemaphoreTake(USBTransferCompleted, portMAX_DELAY);
						USB_SIL_Write(EP1_IN, (uint8_t *)&Player.value, 2);
						SetEPTxValid(ENDP1);
					}
				}
				else if (mMusicPlayer.MusicPlayerConfig.ValueDecrement)
				{
					Player.btns.VolumeDown = 1;
					remain = mMusicPlayer.MusicPlayerConfig.SliderChangedValue;
					while (remain--) // 50
					{
						xSemaphoreTake(USBTransferCompleted, portMAX_DELAY);
						USB_SIL_Write(EP1_IN, (uint8_t *)&Player.value, 2);
						SetEPTxValid(ENDP1);
					}
				}
			}
			else if (mMusicPlayer.MusicPlayerConfig.LightChange)
			{
				if (mMusicPlayer.MusicPlayerConfig.ValueIncrement)
				{
					Player.btns.LightInc = 1;
					remain = mMusicPlayer.MusicPlayerConfig.SliderChangedValue;
					remain = (remain * 10) / 100;
					while (remain--)
					{
						xSemaphoreTake(USBTransferCompleted, portMAX_DELAY);
						USB_SIL_Write(EP1_IN, (uint8_t *)&Player.value, 2);
						SetEPTxValid(ENDP1);
					}
				}
				else if (mMusicPlayer.MusicPlayerConfig.ValueDecrement)
				{
					Player.btns.LightDec = 1;
					remain = mMusicPlayer.MusicPlayerConfig.SliderChangedValue;
					remain = (remain * 10) / 100;
					while (remain--)
					{
						xSemaphoreTake(USBTransferCompleted, portMAX_DELAY);
						USB_SIL_Write(EP1_IN, (uint8_t *)&Player.value, 2);
						SetEPTxValid(ENDP1);
					}
				}
			}
			else if (mMusicPlayer.MusicPlayerConfig.BtnClicked)
			{
				if (mMusicPlayer.MusicPlayerConfig.PlayBtnClicked)
				{
					Player.btns.Play = 1;
				}
				else if (mMusicPlayer.MusicPlayerConfig.PauseBtnClicked)
				{
					Player.btns.Pause = 1;
				}
				else if (mMusicPlayer.MusicPlayerConfig.StopBtnClicked)
				{
					Player.btns.Stop = 1;
				}
				else if (mMusicPlayer.MusicPlayerConfig.MuteBtnClicked)
				{
					Player.btns.Mute = 1;
				}
				else if (mMusicPlayer.MusicPlayerConfig.PreviousTrackBtnClicked)
				{
					Player.btns.Last = 1;
				}
				else if (mMusicPlayer.MusicPlayerConfig.NextTrackBtnClicked)
				{
					Player.btns.Next = 1;
				}
				xSemaphoreTake(USBTransferCompleted, portMAX_DELAY);
				USB_SIL_Write(EP1_IN, (uint8_t *)&Player.value, 2);
				SetEPTxValid(ENDP1);
			}
			Player.value = 0;
			mMusicPlayer.MusicPlayerValue = 0;
			xSemaphoreTake(USBTransferCompleted, portMAX_DELAY);
			USB_SIL_Write(EP1_IN, (uint8_t *)&Player.value, 2);
			SetEPTxValid(ENDP1);
		}
	}
}

void Lvgl_update_task(void *pvParameters)
{
	while (1)
	{
		mutex_lock(lvgl_mutex);
		lv_timer_handler();
		mutex_unlock(lvgl_mutex);
		vTaskDelay(pdMS_TO_TICKS(5));
	}
}
#if DEBUG
char buffer[200];
char MusicStackStr[20];
char LvglUpdateStackStr[20];
#endif
void Detect_task(void *pvParameters)
{
	LED_Init();
	while (1)
	{
		LED2 = !LED2;
#if DEBUG
		vTaskList(buffer);
		printf("%s\r\n", buffer);
#endif

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
