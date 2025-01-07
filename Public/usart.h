#ifndef _usart_H
#define _usart_H

#include "system.h" 
#include "stdio.h" 

#define USART2_REC_LEN		200  	//定义最大接收字节数 200

extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记


void USART2_Init(u32 bound);


#endif


