#ifndef __UART1_H__
#define __UART1_H__
#include "USER.h" 

void UART1_Init(void); //初始化
void UART1_Send(u8 c);  //发送程序
void UART1_String(u8 *data);


#endif    