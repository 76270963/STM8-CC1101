#ifndef __UART1_H__
#define __UART1_H__
#include "USER.h" 

void UART1_Init(void); //��ʼ��
void UART1_Send(u8 c);  //���ͳ���
void UART1_String(u8 *data);


#endif    