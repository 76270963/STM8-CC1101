/*
      
	//auth��CYM
	//date: 2019/9/12

	  ��������
         ����λ 8
	 ������ 9600
	 У��λ N 
	 ֹͣλ 1

*/

#include "IOSTM8S103F3.h"
#include "UART1.h"
#include "USER.h"  //�û�����



//16M    / Ԥ��Ƶֵ 16K
//16000000/16000=1000   1s
u16 time;                         //ms��ʱ
u8 ucTime100MsCount = 0;         //100mS����
u8 ucTime1SCount = 0;            //1S����
u8 ucTime1MCount = 0;            //1min����
u8 ucTime1HCount = 0;            //1h����

u8 RxBuf[2]={0x01,0x02};
u8 PR = 0;

//CC1101������
#define RF_BUFF_SIZE	64			//CC1101��������С
u8 RfBuff[RF_BUFF_SIZE];			//CC1101������
u8 RfLen = 0;						//CC1101���ջ�������С


void GPIO_Init()
{
   PD_DDR_DDR3=1;    //���������
   PD_CR1_C13=1;



   PC_DDR_DDR4=0;      //��ť����
   PC_CR1_C14=1;

    //��ʼ��Ƭѡ  
    PA_DDR_DDR3=1;    
    PA_CR1_C13=1;
    CC1101_CS_H();  

    //��ʼ��SCLK  
    PC_DDR_DDR5=1;    
    PC_CR1_C15=1;
    CC1101_SCLK_H();  
    
    //��ʼ��MOSI  
    PC_DDR_DDR6=1;    
    PC_CR1_C16=1;
    CC1101_MOSI_H();  
    
    //��ʼ��MISO  
    PC_DDR_DDR7=0;      
    PC_CR1_C17=1;
  
    CC1101_SCLK_L();  
    CC1101_MOSI_L();  
    
    //��ʼ��GDO0,GDO2��ӦPD2,PC3 
    PD_DDR_DDR2=0;      
    PD_CR1_C12=1;
    
    //PC3�жϽ���
    PC_DDR_DDR3=0;      
    PC_CR1_C13=1;
    PC_CR2_C23=1;  //�����ж�
    EXTI_CR1_PCIS=1;  //�������ж�

 
}

void TIM1_Init()
{
    TIM1_CR1=0x00;        //TIM1 ��λ
    TIM1_IER=0x01;        //�ж�ʹ�ܼĴ��� UIE��������ж�
    
   /**********************************
     16M / Ԥ��Ƶֵ 16K
     ��ʱ��Ƶ��/Ԥ��Ƶ��ֵ��0x3e80�� 
    16000000/16000=1000   1s
    *********************************/
    TIM1_PSCRH=0x3e;      //Ԥ��Ƶ����8λ
    TIM1_PSCRL=0x80;      //Ԥ��Ƶ����8λ
    
    TIM1_ARRH=0x00;     //�Զ���װ�ؼĴ��� 
    TIM1_ARRL=0x01;
    
    TIM1_CR1|=0x01;     //CENʹ�ܼ�����
    
}



void Delay_ms(u16 time_ms)
{
  time=time_ms;
  while(time)
  {
     IWDG_KR = 0xAA; //ˢ��IWDG��ι����
  }
  
}

//PC3�жϺ����� 

#pragma vector=EXTI2_vector  
__interrupt void EXTI2 (void)
{ 
	PR=1;		
}


//TIM1�жϺ����� 

#pragma vector=TIM1_OVR_UIF_vector 
__interrupt void TIM1_OVR_UIF(void)
{ 
	TIM1_SR1=0x00;  //��־λ����
	if(time)
	{
		time--;
	}
		
	ucTime100MsCount++; 
	if(ucTime100MsCount >= 100)          //100����
	{
		ucTime100MsCount = 0;
        
		ucTime1SCount++;
		if(ucTime1SCount >= 10)           //1����
		{ 
			ucTime1SCount=0;
			
			ucTime1MCount++;
			if(ucTime1MCount >= 60)  	 //1����
			{
				ucTime1MCount=0;

				ucTime1HCount++;
				if(ucTime1HCount >= 60)    //1Сʱ
				{
					ucTime1HCount=0;	
				}
			}
		}
	}
}

//UART1 �жϺ����� 

#pragma vector= UART1_R_OR_vector//0x19
__interrupt void UART1_R_OR_IRQHandler(void)
{
      u8 Data;
      UART1_SR=0;  //��־λ����
      Data=UART1_DR;
      return;
}




//��ȡоƬUID��
u8 Read_UniqueID_Byte(u8 offset)  
{  
    u8 id_byte;

        id_byte = *(u8*)(0x4866+offset);           //ID����ַ��0x4865 

        return id_byte; 
} 

//EEPROMָ����ַ����һ������
u8 Read_EEPROM_Byte(u8  addr)
 {
   u8 *p;
   p=( u8  *)(0x4000+addr); 
   return *p; 
 }

//дEEPROM   ��ʼ��ַ  0x4000

u8 Write_EEPROM_Byte(u8 offset, u8 data)  
{  
    FLASH_DUKR=0xAE; 
    FLASH_DUKR=0x56;  
    FLASH_CR2=0x00;  
    FLASH_NCR2=0xFF;
    if(!(FLASH_IAPSR & 0x08)) //����Ӧ��λ�Ƿ����  
        return 0;  
    *(u8*)(0x4000+offset)=data;
    FLASH_IAPSR=(u8)(~0x08); //��������   
    return 1; 
} 


//���Ź�����  
void IWDG_Init(void)
{
    IWDG_KR  = 0xCC; //��IWDG_KR��д����ֵ0xCC���������Ź���������
    IWDG_KR  = 0x55;  //��� PR �� RLR ��д����
    IWDG_RLR = 0xff; //���Ź���������װ����ֵ         
    IWDG_PR  = 0x06; //��Ƶϵ��Ϊ256�����ʱʱ��Ϊ��1.02S
    IWDG_KR  = 0xAA; //�ָ� PR �� RLR ��д����״̬
}




void main(void)
{
    asm("sim");  //�������ж�
    CLK_CKDIVR=0x00;  //16M
    GPIO_Init();
    TIM1_Init();
    //UART1_Init(); 
    IWDG_Init();
    CC1101_Init(0Xaa);
    asm("rim");  //�������ж�

    
    
    //CC1101_WriteReg(CC1101_REG_PATABLE1,0xc0);
 
    CC1101_WriteReg(CC1101_REG_PATABLE0,0x00);
    CC1101_Command(CC1101_CMD_SIDLE);   //�˳���ǰģʽ
    CC1101_Command(CC1101_CMD_SRX);     //�������ģʽ

  
     //**********************************************
    while (1)
    {
        if(Button==0)
        {
           
            
            CC1101_WriteReg(CC1101_REG_PATABLE0,0xc0);//������������	
            CC1101_RfDataSendPack(RxBuf, 2);//CC1101�������ݰ�
				
            CC1101_Init(0Xaa);
            CC1101_WriteReg(CC1101_REG_PATABLE0,0);	//�ر���������
            CC1101_Command(CC1101_CMD_SIDLE);   	//�˳���ǰģʽ
            CC1101_Command(CC1101_CMD_SRX);     	//�������ģʽ
            
        }
 
        
        if(PR)
        {
            RfLen = CC1101_ReadReg(CC1101_REG_RXBYTES);  //��ȡ���ݳ���
            CC1101_ReadRxFIFO(RfBuff,RfLen);             //��ȡ����
             if((RfBuff[1]==0x01)&&(RfBuff[2]==0x02))
            {
              RfBuff[1]=0;
              RfBuff[2]=0;
              Buzzer=1;
              Delay_ms(30);
              Buzzer=0;
              Delay_ms(30);
            }
            CC1101_Command(CC1101_CMD_SIDLE);   //�˳���ǰģʽ
            CC1101_Command(CC1101_CMD_SFRX);   	//������ջ�����
	    CC1101_Command(CC1101_CMD_SRX);     //�������ģʽ
            
            
            PR=0;
        }
        
       IWDG_KR = 0xAA; //ˢ��IWDG��ι����
    }
}



