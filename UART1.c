/*******************************
     UART1.C ����
 
******************************/

#include "IOSTM8S103F3.h"
#include "USER.h" 
#include "UART1.h"


void UART1_Init(void)
{
      UART1_CR1= 0x00;
      UART1_CR2= 0x00;
      UART1_CR3= 0x00;
      //����������  ��ʱ�ӣ�16M 16000000/19200=833(0x341)
      // ������дBRR2   
      // ��Ӧ��ʮ��������Ϊ0341��BBR1=34,BBR2=01
      UART1_BRR2= 0x03;
      UART1_BRR1= 0x68;
      UART1_CR2 = 0x2c;  // Ob0010 1100
      //�����жϣ� ���ͱ�ʹ�ܣ� ���ձ�ʹ��    
}

void UART1_Send(u8 c)
{
      while((UART1_SR & 0x80)==0x00);
      UART1_DR=c;
      while((UART1_SR & 0x40)==0x00);
}


int  putchar(int c)
 {
     UART1_DR=c;;
     while((UART1_SR & 0x80)==0x00);
     return(c);
 }