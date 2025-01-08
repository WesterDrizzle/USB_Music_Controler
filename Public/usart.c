#include "usart.h"

int fputc(int ch, FILE *p)
{
	USART_SendData(USART2, (u8)ch);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		;
	return ch;
}

u8 USART2_RX_BUF[USART2_REC_LEN];
u16 USART2_RX_STA = 0;

void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE);

	USART_ClearFlag(USART2, USART_FLAG_TC);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_IRQHandler(void)
{
	u8 r;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		r = USART_ReceiveData(USART2);
		if ((USART2_RX_STA & 0x8000) == 0)
		{
			if (USART2_RX_STA & 0x4000)
			{
				if (r != 0x0a)
					USART2_RX_STA = 0;
				else
					USART2_RX_STA |= 0x8000;
			}
			else
			{
				if (r == 0x0d)
					USART2_RX_STA |= 0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = r;
					USART2_RX_STA++;
					if (USART2_RX_STA > (USART2_REC_LEN - 1))
						USART2_RX_STA = 0;
				}
			}
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
