/*
      
	//auth：CYM
	//date: 2019/9/12

	  串口配置
         数据位 8
	 波特率 9600
	 校验位 N 
	 停止位 1

*/

#include "IOSTM8S103F3.h"
#include "UART1.h"
#include "USER.h"  //用户定义



//16M    / 预分频值 16K
//16000000/16000=1000   1s
u16 time;                         //ms计时
u8 ucTime100MsCount = 0;         //100mS计数
u8 ucTime1SCount = 0;            //1S计数
u8 ucTime1MCount = 0;            //1min计数
u8 ucTime1HCount = 0;            //1h计数

u8 RxBuf[2]={0x01,0x02};
u8 PR = 0;

//CC1101缓冲区
#define RF_BUFF_SIZE	64			//CC1101缓冲区大小
u8 RfBuff[RF_BUFF_SIZE];			//CC1101缓冲区
u8 RfLen = 0;						//CC1101接收缓冲区大小


void GPIO_Init()
{
   PD_DDR_DDR3=1;    //蜂鸣器输出
   PD_CR1_C13=1;



   PC_DDR_DDR4=0;      //按钮输入
   PC_CR1_C14=1;

    //初始化片选  
    PA_DDR_DDR3=1;    
    PA_CR1_C13=1;
    CC1101_CS_H();  

    //初始化SCLK  
    PC_DDR_DDR5=1;    
    PC_CR1_C15=1;
    CC1101_SCLK_H();  
    
    //初始化MOSI  
    PC_DDR_DDR6=1;    
    PC_CR1_C16=1;
    CC1101_MOSI_H();  
    
    //初始化MISO  
    PC_DDR_DDR7=0;      
    PC_CR1_C17=1;
  
    CC1101_SCLK_L();  
    CC1101_MOSI_L();  
    
    //初始化GDO0,GDO2对应PD2,PC3 
    PD_DDR_DDR2=0;      
    PD_CR1_C12=1;
    
    //PC3中断接收
    PC_DDR_DDR3=0;      
    PC_CR1_C13=1;
    PC_CR2_C23=1;  //开启中断
    EXTI_CR1_PCIS=1;  //上升沿中断

 
}

void TIM1_Init()
{
    TIM1_CR1=0x00;        //TIM1 复位
    TIM1_IER=0x01;        //中断使能寄存器 UIE允许更新中断
    
   /**********************************
     16M / 预分频值 16K
     主时钟频率/预分频器值（0x3e80） 
    16000000/16000=1000   1s
    *********************************/
    TIM1_PSCRH=0x3e;      //预分频器高8位
    TIM1_PSCRL=0x80;      //预分频器低8位
    
    TIM1_ARRH=0x00;     //自动重装载寄存器 
    TIM1_ARRL=0x01;
    
    TIM1_CR1|=0x01;     //CEN使能计数器
    
}



void Delay_ms(u16 time_ms)
{
  time=time_ms;
  while(time)
  {
     IWDG_KR = 0xAA; //刷新IWDG（喂狗）
  }
  
}

//PC3中断函数； 

#pragma vector=EXTI2_vector  
__interrupt void EXTI2 (void)
{ 
	PR=1;		
}


//TIM1中断函数； 

#pragma vector=TIM1_OVR_UIF_vector 
__interrupt void TIM1_OVR_UIF(void)
{ 
	TIM1_SR1=0x00;  //标志位清零
	if(time)
	{
		time--;
	}
		
	ucTime100MsCount++; 
	if(ucTime100MsCount >= 100)          //100毫秒
	{
		ucTime100MsCount = 0;
        
		ucTime1SCount++;
		if(ucTime1SCount >= 10)           //1秒钟
		{ 
			ucTime1SCount=0;
			
			ucTime1MCount++;
			if(ucTime1MCount >= 60)  	 //1分钟
			{
				ucTime1MCount=0;

				ucTime1HCount++;
				if(ucTime1HCount >= 60)    //1小时
				{
					ucTime1HCount=0;	
				}
			}
		}
	}
}

//UART1 中断函数； 

#pragma vector= UART1_R_OR_vector//0x19
__interrupt void UART1_R_OR_IRQHandler(void)
{
      u8 Data;
      UART1_SR=0;  //标志位清零
      Data=UART1_DR;
      return;
}




//获取芯片UID码
u8 Read_UniqueID_Byte(u8 offset)  
{  
    u8 id_byte;

        id_byte = *(u8*)(0x4866+offset);           //ID基地址是0x4865 

        return id_byte; 
} 

//EEPROM指定地址读出一个数据
u8 Read_EEPROM_Byte(u8  addr)
 {
   u8 *p;
   p=( u8  *)(0x4000+addr); 
   return *p; 
 }

//写EEPROM   起始地址  0x4000

u8 Write_EEPROM_Byte(u8 offset, u8 data)  
{  
    FLASH_DUKR=0xAE; 
    FLASH_DUKR=0x56;  
    FLASH_CR2=0x00;  
    FLASH_NCR2=0xFF;
    if(!(FLASH_IAPSR & 0x08)) //检测对应的位是否解锁  
        return 0;  
    *(u8*)(0x4000+offset)=data;
    FLASH_IAPSR=(u8)(~0x08); //重新上锁   
    return 1; 
} 


//看门狗配置  
void IWDG_Init(void)
{
    IWDG_KR  = 0xCC; //当IWDG_KR被写入数值0xCC，独立看门狗被启动了
    IWDG_KR  = 0x55;  //解除 PR 及 RLR 的写保护
    IWDG_RLR = 0xff; //看门狗计数器重装载数值         
    IWDG_PR  = 0x06; //分频系数为256，最长超时时间为：1.02S
    IWDG_KR  = 0xAA; //恢复 PR 及 RLR 的写保护状态
}




void main(void)
{
    asm("sim");  //关所有中断
    CLK_CKDIVR=0x00;  //16M
    GPIO_Init();
    TIM1_Init();
    //UART1_Init(); 
    IWDG_Init();
    CC1101_Init(0Xaa);
    asm("rim");  //开所有中断

    
    
    //CC1101_WriteReg(CC1101_REG_PATABLE1,0xc0);
 
    CC1101_WriteReg(CC1101_REG_PATABLE0,0x00);
    CC1101_Command(CC1101_CMD_SIDLE);   //退出当前模式
    CC1101_Command(CC1101_CMD_SRX);     //进入接收模式

  
     //**********************************************
    while (1)
    {
        if(Button==0)
        {
           
            
            CC1101_WriteReg(CC1101_REG_PATABLE0,0xc0);//开启天线增益	
            CC1101_RfDataSendPack(RxBuf, 2);//CC1101发送数据包
				
            CC1101_Init(0Xaa);
            CC1101_WriteReg(CC1101_REG_PATABLE0,0);	//关闭天线增益
            CC1101_Command(CC1101_CMD_SIDLE);   	//退出当前模式
            CC1101_Command(CC1101_CMD_SRX);     	//进入接收模式
            
        }
 
        
        if(PR)
        {
            RfLen = CC1101_ReadReg(CC1101_REG_RXBYTES);  //读取数据长度
            CC1101_ReadRxFIFO(RfBuff,RfLen);             //读取数据
             if((RfBuff[1]==0x01)&&(RfBuff[2]==0x02))
            {
              RfBuff[1]=0;
              RfBuff[2]=0;
              Buzzer=1;
              Delay_ms(30);
              Buzzer=0;
              Delay_ms(30);
            }
            CC1101_Command(CC1101_CMD_SIDLE);   //退出当前模式
            CC1101_Command(CC1101_CMD_SFRX);   	//清除接收缓冲区
	    CC1101_Command(CC1101_CMD_SRX);     //进入接收模式
            
            
            PR=0;
        }
        
       IWDG_KR = 0xAA; //刷新IWDG（喂狗）
    }
}



