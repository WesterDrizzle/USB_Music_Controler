#include "usart.h"		 

int fputc(int ch,FILE *p)  //º¯ʽĬȏµģ¬ԚʹӃprintfº¯ʽʱה¶¯µ÷Ӄ
{
	USART_SendData(USART2,(u8)ch);	
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	return ch;
}

//´®¿ڱ֐¶Ϸþα³̐򍊯/עҢ,¶ÁȡUSARTx->SRĜ±܃KûƤîµĴ펳   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //½ӊջº³嬗USART_REC_LEN¸öז½ڮ
//½ӊ՗´̬
//bit15£¬	½ӊՍ곉±ꖾ
//bit14£¬	½ӊյ½0x0d
//bit13~0£¬	½ӊյ½µēА§ז½ڊýĿ
u16 USART2_RX_STA=0;       //½ӊ՗´̬±꼇


/*******************************************************************************
* º¯ ʽ û         : USART2_Init
* º¯ʽ¹¦Ĝ		   : USART2³õʼ»¯º¯ʽ
* ʤ    ȫ         : bound:²¨̘
* ʤ    ³ö         : Ξ
*******************************************************************************/ 
void USART2_Init(u32 bound)
{
   //GPIO¶˿ډ薃
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	
	/*  ŤփGPIOµĄ£ʽº͉O¿ڠ*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TX			   //´®¿ڊ䳶PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //¸´Ӄ͆ͬʤ³ö
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ³õʼ»¯´®¿ڊ䈫IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//RX			 //´®¿ڊ䈫PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģĢʤȫ
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ³õʼ»¯GPIO */
	

	//USART2 ³õʼ»¯ɨփ
	USART_InitStructure.USART_BaudRate = bound;//²¨̘ɨփ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//ז³¤Ϊ8λʽ¾ݸ񊽍
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ¸öֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//ΞƦżУѩλ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ΞӲ¼þʽ¾݁÷¿ؖƍ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ʕ·¢ģʽ
	USART_Init(USART2, &USART_InitStructure); //³õʼ»¯´®¿ڱ
	
	USART_Cmd(USART2, ENABLE);  //ʹĜ´®¿ڱ 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//¿ªƴϠ¹ؖжύ

	//Usart1 NVIC Ťփ
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ݖ¸¶¨µĲΊý³õʼ»¯VIC¼Ĵ憷¡¢	
}

/*******************************************************************************
* º¯ ʽ û         : USART2_IRQHandler
* º¯ʽ¹¦Ĝ		   : USART2֐¶Ϻ¯ʽ
* ʤ    ȫ         : Ξ
* ʤ    ³ö         : Ξ
*******************************************************************************/ 
void USART2_IRQHandler(void)                	//´®¿ڱ֐¶Ϸþα³̐򍊻
{
	u8 r;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //½ӊՖжύ
	{
		r =USART_ReceiveData(USART2);//(USART2->DR);	//¶Áȡ½ӊյ½µĊý¾ݍ
		if((USART2_RX_STA&0x8000)==0)//½ӊՎ´ͪ³ɍ
		{
			if(USART2_RX_STA&0x4000)//½ӊյ½Á˰x0d
			{
				if(r!=0x0a)USART2_RX_STA=0;//½ӊմ펳,֘Ђ¿ªʼ
				else USART2_RX_STA|=0x8000;	//½ӊՍ곉Áˠ
			}
			else //»¹ûʕµ½0X0D
			{	
				if(r==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=r;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//½ӊՊý¾ݴ펳,֘Ђ¿ªʼ½ӊՉ  
				}		 
			}
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);		
	} 
}
