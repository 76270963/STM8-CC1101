/*************************************************************************************************************
     CC1101.C 驱动
*************************************************************************************************************/  

#include "IOSTM8S103F3.h"
#include "USER.h"   //用户定义 
  
//CC1101命令掩码  
#define     WRITE_BURST         0x40                        //连续写入  
#define     READ_SINGLE         0x80                        //读  
#define     WRITE_SINGLE        0x00                        //写  
#define     READ_BURST          0xC0  
  
  
  
//连续读  
#define     BURST_READ_FIFO     0xff        //突发读取RX FIFO  
#define     BYTE_READ_FIFO      0xBF        //单字节读取 RX FIFO  
#define     BURST_WRITE_FIFO    0x7f        //突发写TX FIFO  
#define     BYTE_WRITE_FIFO     0x3f        //单字节写 TX FIFO  
  
#define     CC1101_DATA_LEN     64  
  
  
  

  
//CC1101 SPI读写一字节  
//不带片选  
u8 CC1101_ReadWriteByte(u8 data)  
{  
    u8 i;  
    u8 temp = 0;  
  
    for(i = 0;i < 8;i ++)  
    {  
        if(data & 0x80)  
        {  
            CC1101_MOSI_H();  
        }  
        else  
        {  
            CC1101_MOSI_L();  
        }  
        data <<= 1; asm("NOP");  
        CC1101_SCLK_H();                //时钟上升沿写入数据  
        temp <<= 1;asm("NOP"); 
        if(CC1101_GetMISO) temp ++;  
        CC1101_SCLK_L();                //时钟下降沿读取数据  
    }  
  
    return temp;  
}  
  
  
/*************************************************************************************************************************
* 函数    :   u8 CC1101_Command(CC1101_CMD_TYPE Cmd)
* 功能    :   发送单字节命令
* 参数    :   Cmd;命令,见CC1101_CMD_TYPE
* 返回    :   寄存器的值
* 依赖    :   底层宏定义
* 说明    :   以写的方式单直接访问将触发响应的命令
*************************************************************************************************************************/  
u8 CC1101_Command(CC1101_CMD_TYPE Cmd)  
{  
    u8 status;  
  
    CC1101_CS_L();                              //片选有效  
    while(CC1101_GetMISO);  
    status = CC1101_ReadWriteByte((u8)Cmd);     //发送命令  
    while(CC1101_GetMISO);  
    CC1101_CS_H();                              //片选关闭  
    return status;  
}  
  
  
  
/*************************************************************************************************************************
* 函数    :   u8 CC1101_ReadReg(CC1101_REG_TYPE RegAddr)
* 功能    :   读取CC1101通用寄存器
* 参数    :   RegAddr:寄存器地址,见:CC1101_REG_TYPE
* 返回    :   寄存器的值
* 依赖    :   底层宏定义
* 说明    :   一次读取一个寄存器
*************************************************************************************************************************/  
u8 CC1101_ReadReg(CC1101_REG_TYPE RegAddr)  
{  
    u8 data;  
  
    CC1101_CS_L();                                  //片选有效  
    CC1101_ReadWriteByte((u8)READ_SINGLE|RegAddr);  //发送读命令以及寄存器索引  
    data = CC1101_ReadWriteByte(0xff);              //读取  
    CC1101_CS_H();                                  //片选关闭  
    return data;  
}  
  
  
/*************************************************************************************************************************
* 函数    :   u8 CC1101_WriteReg(CC1101_REG_TYPE RegAddr, u8 data)
* 功能    :   写入CC1101通用寄存器
* 参数    :   RegAddr:寄存器地址,见:CC1101_REG_TYPE,data:需要写入的数据
* 返回    :   状态寄存器的值
* 依赖    :   底层宏定义
* 说明    :   一次写入一个寄存器,并返回状态
            不要对只读的寄存器进行写操作
*************************************************************************************************************************/  
u8 CC1101_WriteReg(CC1101_REG_TYPE RegAddr, u8 data)  
{  
    u8 status;  
  
    if(RegAddr > 0x80) return 0;                             //防止误操作,0x30以后的寄存器为只读状态寄存器  
    CC1101_CS_L();                                              //片选有效  
    while(CC1101_GetMISO);  
    status = CC1101_ReadWriteByte((u8)WRITE_SINGLE|RegAddr);    //发送写命令以及寄存器索引  
    CC1101_ReadWriteByte(data);                                 //写入数据  
    CC1101_CS_H();                                              //片选关闭  
    return status;  
}  
  
  

void CC1101_Init(u8 Addr)  
{  
  
    //初始化寄存器  
    CC1101_Command(CC1101_CMD_SRES);                    //复位  
    //Delay_ms(10);  
      
    while(CC1101_ReadReg(CC1101_REG_AGCTEST) != 0x3F)   //检测通信  
    {  
        //LED_ON();  
        //Delay_MS(10);  
        //LED_OFF();  
        //Delay_MS(100);  
    }  
    //LED_OFF();  
  
    CC1101_WriteReg(CC1101_REG_IOCFG0,0x06);            //发送提示引脚  
    CC1101_WriteReg(CC1101_REG_IOCFG2,0x01);            //接收提示引脚  
  
    CC1101_WriteReg(CC1101_REG_FIFOTHR,0x0f);           //RX FIFO和TX FIFO门限  
    CC1101_WriteReg(CC1101_REG_SYNC1,0xD3);             //同步词汇，高字节  
    CC1101_WriteReg(CC1101_REG_SYNC0,0x91);             //同步词汇，低字节  
    CC1101_WriteReg(CC1101_REG_PKTLEN,CC1101_DATA_LEN); //数据包长度,
    CC1101_WriteReg(CC1101_REG_PKTCTRL1,0x04);          //数据包自动控制  
    CC1101_WriteReg(CC1101_REG_PKTCTRL0,0x04);          //数据包自动控制  
    CC1101_WriteReg(CC1101_REG_ADDR,0x00);              //设备地址  
    CC1101_WriteReg(CC1101_REG_CHANNR,0x00);            //信道  
    CC1101_WriteReg(CC1101_REG_FSCTRL1,0x06);           //频率合成器控制，高字节  
    CC1101_WriteReg(CC1101_REG_FSCTRL0,0x00);           //频率合成器控制，低字节  
    CC1101_WriteReg(CC1101_REG_FREQ2,0x10);             //频率控制词汇，高字节  
    CC1101_WriteReg(CC1101_REG_FREQ1,0xb1);             //频率控制词汇，中间字节  
    CC1101_WriteReg(CC1101_REG_FREQ0,0x3b);             //频率控制词汇，低字节  
      
    //2.4KBPS  
    CC1101_WriteReg(CC1101_REG_MDMCFG4,0xF6);           //调制器配置  
    CC1101_WriteReg(CC1101_REG_MDMCFG3,0x83);           //调制器配置  
      
    CC1101_WriteReg(CC1101_REG_MDMCFG2,0x13);           //调制器配置  
    CC1101_WriteReg(CC1101_REG_MDMCFG1,0x22);           //调制器配置  
    CC1101_WriteReg(CC1101_REG_MDMCFG0,0xf8);           //调制器配置  
      
    CC1101_WriteReg(CC1101_REG_DEVIATN,0x15);           //调制器背离设置  
    CC1101_WriteReg(CC1101_REG_MCSM2,0x07);             //主通信控制状态机配置  
    CC1101_WriteReg(CC1101_REG_MCSM1,0x30);             //主通信控制状态机配置  
    CC1101_WriteReg(CC1101_REG_MCSM0,0x18);             //主通信控制状态机配置  
    CC1101_WriteReg(CC1101_REG_FOCCFG,0x16);            //频率偏移补偿配置  
    CC1101_WriteReg(CC1101_REG_BSCFG,0x6c);             //位同步配置  
    CC1101_WriteReg(CC1101_REG_AGCTRL2,0x03);           //AGC控制  
    CC1101_WriteReg(CC1101_REG_AGCTRL1,0x40);           //AGC控制  
    CC1101_WriteReg(CC1101_REG_AGCTRL0,0x91);           //AGC控制  
    CC1101_WriteReg(CC1101_REG_WOREVT1,0x87);           //高字节时间0暂停  
    CC1101_WriteReg(CC1101_REG_WOREVT0,0x6b);           //低字节时间0暂停  
    CC1101_WriteReg(CC1101_REG_WORCTRL,0xfb);           //电磁波激活控制  
    CC1101_WriteReg(CC1101_REG_FREND1,0x56);            //前末端RX配置  
    CC1101_WriteReg(CC1101_REG_FREND0,0x10);            //前末端TX配置  
    CC1101_WriteReg(CC1101_REG_FSCAL3,0xe9);            //频率合成器校准  
    CC1101_WriteReg(CC1101_REG_FSCAL2,0x2a);            //频率合成器校准  
    CC1101_WriteReg(CC1101_REG_FSCAL1,0x00);            //频率合成器校准  
    CC1101_WriteReg(CC1101_REG_FSCAL0,0x1f);            //频率合成器校准  
    CC1101_WriteReg(CC1101_REG_RCCTRL1,0x41);           //RC振荡器配置  
    CC1101_WriteReg(CC1101_REG_RCCTRL0,0x00);           //RC振荡器配置  
    CC1101_WriteReg(CC1101_REG_FSTEST,0x59);            //频率合成器校准控制  
      
    //10DB  
    //CC1101_WriteReg(CC1101_REG_PATABLE0,0xc0);   
    //CC1101_WriteReg(CC1101_REG_PATABLE1,0xc0);   
    /*CC1101_WriteReg(CC1101_REG_PATABLE2,0xc0);  
    CC1101_WriteReg(CC1101_REG_PATABLE3,0xc0);  
    CC1101_WriteReg(CC1101_REG_PATABLE4,0xc0);  
    CC1101_WriteReg(CC1101_REG_PATABLE5,0xc0);  
    CC1101_WriteReg(CC1101_REG_PATABLE6,0xc0);  
    CC1101_WriteReg(CC1101_REG_PATABLE7,0xc0); */  
    //Delay_ms(10);  
}  
  
  
  
/*************************************************************************************************************************
* 函数    :   void CC1101_WriteTxFIFO(u8 *pBuff,u8 len)
* 功能    :   写入数据到发送缓冲区
* 参数    :   pBuff:需要写入的数据缓冲区指针,len:需要写入的数据长度
* 返回    :   无
* 依赖    :   底层宏定义
* 说明    :   写入数据到发送FIFO
*************************************************************************************************************************/  
void CC1101_WriteTxFIFO(u8 *pBuff,u8 len)  
{  
    u16 i;
 
    CC1101_CS_L();
    CC1101_ReadWriteByte(BURST_WRITE_FIFO);
    for(i = 0;i < len;i ++)
    {
        CC1101_ReadWriteByte(pBuff[i]);
    }
    CC1101_CS_H();
}  
  
  
  
/*************************************************************************************************************************
* 函数    :   void CC1101_ReadRxFIFO(u8 *pBuff,u8 len)
* 功能    :   读取接收缓冲区
* 参数    :   pBuff:需要读取的数据缓冲区指针,len:需要读取的数据长度
* 返回    :   无
* 依赖    :   底层宏定义
* 说明    :   从接收FIFO读取数据
*************************************************************************************************************************/  
void CC1101_ReadRxFIFO(u8 *pBuff,u8 len)  
{  
    u16 i;
 
    CC1101_CS_L();
    CC1101_ReadWriteByte(BURST_READ_FIFO);
    for(i = 0;i < len;i ++)
    {
        pBuff[i] = CC1101_ReadWriteByte(0xff);
    }
    CC1101_CS_H();
}  
  
  
//发送数据,将缓冲区数据全部发送出去  
//一次最多64B,因为受到FIFO限制  
void CC1101_RfDataSend(u8 *pBuff,u8 len)  
{  
    CC1101_Command(CC1101_CMD_SIDLE);   //退出当前模式  
    CC1101_Command(CC1101_CMD_SFTX);    //清空发送缓冲区  
    CC1101_WriteTxFIFO(pBuff, len);     //写入数据到发送缓冲区  
    CC1101_Command(CC1101_CMD_STX);     //开始发送数据  
      
    while(!CC1101_GDO0);  
    while(CC1101_GDO0);  
  
    CC1101_Command(CC1101_CMD_SIDLE);   //退出当前模式  
}  
  
  
  
  
  
//发送数据包  
//每次发送最多65B,第一字节为长度,数据多将会重复发送  
//可以发送任意大小  
//CC1101PackSize有效数据包大小,0-64,也就是CC1101单次发送数据大小-1  
void CC1101_RfDataSendPack(u8 *pBuff, u16 len)  
{  
    u16 i,m,n,j;  
      
    m = len / (CC1101_DATA_LEN-1);          //整数数据帧数量  
    n = len % (CC1101_DATA_LEN-1);          //余数  
      
    //发送整数包  
    for(i = 0;i < m;i ++)                  
    {  
        Delay_ms(1);  
        CC1101_Command(CC1101_CMD_SIDLE);   //退出当前模式  
        CC1101_Command(CC1101_CMD_SFTX);    //清空发送缓冲区  
         
        CC1101_CS_L();  
        CC1101_ReadWriteByte(BURST_WRITE_FIFO);  
         
        CC1101_ReadWriteByte(CC1101_DATA_LEN-1);//先写入包大小      
        for(j = 0;j < (CC1101_DATA_LEN-1);j ++)  
        {  
            CC1101_ReadWriteByte(*pBuff++); //写入数据到发送缓冲区  
        }  
        CC1101_CS_H();  
         
        CC1101_Command(CC1101_CMD_STX);     //开始发送数据  
        while(!CC1101_GDO0);  
        while(CC1101_GDO0);                 //等待发送完成  
    }  
    //发送余数包  
    if(n!=0)  
    {  
        Delay_ms(1);  
        CC1101_Command(CC1101_CMD_SIDLE);   //退出当前模式  
        CC1101_Command(CC1101_CMD_SFTX);    //清空发送缓冲区  
        CC1101_CS_L();  
        CC1101_ReadWriteByte(BURST_WRITE_FIFO);  
         
        CC1101_ReadWriteByte(n);            //先写入包大小      
        for(j = 0;j < n;j ++)  
        {  
            CC1101_ReadWriteByte(*pBuff++); //写入数据到发送缓冲区  
        }  
        CC1101_CS_H();  
         
        CC1101_Command(CC1101_CMD_STX);     //开始发送数据  
        while(!CC1101_GDO0);  
        while(CC1101_GDO0);                 //等待发送完成  
    }  
    CC1101_Command(CC1101_CMD_SIDLE);   //退出当前模式  
}  
  
  
  
  
  
  
  
//读取芯片状态  
u8 CC1101_GetStatus(void)  
{  
    return CC1101_WriteReg(CC1101_REG_TEST2, 0x98);  
}