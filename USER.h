#ifndef __USER_H__
#define __USER_H__



#define u8   unsigned char	
#define u16  unsigned int
#define u32  unsigned long
#define i8   unsigned short
#define i16  unsigned int





#define  Buzzer            PD_ODR_ODR3   //蜂鸣器
#define  Button            PC_IDR_IDR4   //按钮输入
#define  CC1101_GDO0       PD_IDR_IDR2   //PD2
#define  CC1101_GDO2       PC_IDR_IDR3   //PC3




//SPI接口  
//底层接口宏定义  
#define CC1101_CS_H()                PA_ODR_ODR3=1           //PA3=1  
#define CC1101_CS_L()                PA_ODR_ODR3=0           //PA3=0  
#define CC1101_MOSI_H()              PC_ODR_ODR6=1           //PC6  
#define CC1101_MOSI_L()              PC_ODR_ODR6=0           //PC6  
#define CC1101_SCLK_H()              PC_ODR_ODR5=1           //PC5  
#define CC1101_SCLK_L()              PC_ODR_ODR5=0           //PC5  
#define CC1101_GetMISO               PC_IDR_IDR7              //PC7  
  


//CC1101 命令
//以写的方式单直接访问将触发响应的命令
typedef enum
{
	CC1101_CMD_SRES		=	0x30,	//重启
	CC1101_CMD_SFSTXON	=	0x31,	//开启和校准频率合成器（若MCSM0.FSAUTOCAL=1）
	CC1101_CMD_SXOFF	=	0x32,	//关闭晶体振荡器
	CC1101_CMD_SCAL		=	0x33,	//校准频率合成器并关断（开启快速启动）。在不设置手动校准模式（MCSM0.FS_AUTOCAL=0）的情况下，SCAL能从空闲模式滤波。
	CC1101_CMD_SRX		=	0x34,	//启用RX。若上一状态为空闲且MCSM0.FS_AUTOCAL=1则首先运行校准。
	CC1101_CMD_STX		=	0x35,	//空闲状态：启用TX。若MCSM0.FS_AUTOCAL=1首先运行校准。若在RX状态且CCA启用：若信道为空则进入TX
	CC1101_CMD_SIDLE	=	0x36,	//离开RX/TX,关断频率合成器并离开电磁波激活模式若可用
	CC1101_CMD_SAFC		=	0x37,	//运行22.1节列出的频率合成器的AFC调节
	CC1101_CMD_SWOR		=	0x38,	//运行27.5节描述的自动RX选举序列（电磁波激活）
	CC1101_CMD_SPWD		=	0x39,	//当CSn为高时进入功率降低模式。
	CC1101_CMD_SFRX		=	0x3a,	//冲洗RX FIFO缓冲
	CC1101_CMD_SFTX		=	0x3b,	//冲洗TX FIFO缓冲
	CC1101_CMD_SWORRST	=	0x3c,	//重新设置真实时间时钟
	CC1101_CMD_SNOP		=	0x3d,	//无操作。可能用来为更简单的软件将滤波命令变为2字节。
}CC1101_CMD_TYPE;
 
 
 
//CC1101寄存器定义
typedef enum
{
	//可读写的寄存器
	CC1101_REG_IOCFG2		=	0x00,	//GDO2输出脚配置
	CC1101_REG_IOCFG1		=	0x01,	//GDO1输出脚配置
	CC1101_REG_IOCFG0		=	0x02,	//GDO0输出脚配置
	CC1101_REG_FIFOTHR		=	0x03,	//RX FIFO和TX FIFO门限
	CC1101_REG_SYNC1		=	0x04,	//同步词汇，高字节
	CC1101_REG_SYNC0		=	0x05,	//同步词汇，低字节
	CC1101_REG_PKTLEN		=	0x06,	//数据包长度
	CC1101_REG_PKTCTRL1		=	0x07,	//数据包自动控制
	CC1101_REG_PKTCTRL0		=	0x08,	//数据包自动控制
	CC1101_REG_ADDR			=	0x09,	//设备地址
	CC1101_REG_CHANNR		=	0x0a,	//信道数
	CC1101_REG_FSCTRL1		=	0x0b,	//频率合成器控制，高字节
	CC1101_REG_FSCTRL0		=	0x0c,	//频率合成器控制，低字节
	CC1101_REG_FREQ2		=	0x0d,	//频率控制词汇，高字节
	CC1101_REG_FREQ1		=	0x0e,	//频率控制词汇，中间字节
	CC1101_REG_FREQ0		=	0x0f,	//频率控制词汇，低字节
	CC1101_REG_MDMCFG4		=	0x10,	//调制器配置
	CC1101_REG_MDMCFG3		=	0x11,	//调制器配置
	CC1101_REG_MDMCFG2		=	0x12,	//调制器配置
	CC1101_REG_MDMCFG1		=	0x13,	//调制器配置
	CC1101_REG_MDMCFG0		=	0x14,	//调制器配置
	CC1101_REG_DEVIATN		=	0x15,	//调制器背离设置
	CC1101_REG_MCSM2		=	0x16,	//主通信控制状态机配置
	CC1101_REG_MCSM1		=	0x17,	//主通信控制状态机配置
	CC1101_REG_MCSM0		=	0x18,	//主通信控制状态机配置
	CC1101_REG_FOCCFG		=	0x19,	//频率偏移补偿配置
	CC1101_REG_BSCFG		=	0x1a,	//位同步配置
	CC1101_REG_AGCTRL2		=	0x1b,	//AGC控制
	CC1101_REG_AGCTRL1		=	0x1c,	//AGC控制
	CC1101_REG_AGCTRL0		=	0x1d,	//AGC控制
	CC1101_REG_WOREVT1		=	0x1e,	//高字节时间0暂停
	CC1101_REG_WOREVT0		=	0x1f,	//低字节时间0暂停
	CC1101_REG_WORCTRL		=	0x20,	//电磁波激活控制
	CC1101_REG_FREND1		=	0x21,	//前末端RX配置
	CC1101_REG_FREND0		=	0x22,	//前末端TX配置
	CC1101_REG_FSCAL3		=	0x23,	//频率合成器校准
	CC1101_REG_FSCAL2		=	0x24,	//频率合成器校准
	CC1101_REG_FSCAL1		=	0x25,	//频率合成器校准
	CC1101_REG_FSCAL0		=	0x26,	//频率合成器校准
	CC1101_REG_RCCTRL1		=	0x27,	//RC振荡器配置
	CC1101_REG_RCCTRL0		=	0x28,	//RC振荡器配置
	CC1101_REG_FSTEST		=	0x29,	//频率合成器校准控制
	CC1101_REG_PTEST		=	0x2a,	//产品测试
	CC1101_REG_AGCTEST		=	0x2b,	//AGC测试
	CC1101_REG_TEST2		=	0x2c,	//不同的测试设置
	CC1101_REG_TEST1		=	0x2d,	//不同的测试设置
	CC1101_REG_TEST0		=	0x2e,	//不同的测试设置
	//只读的状态寄存器,如果写入将导致命令滤波
	CC1101_REG_PARTNUM		=	0xf0,	//CC2550的组成部分数目
	CC1101_REG_VERSION		=	0xf1,	//当前版本数
	CC1101_REG_FREQEST		=	0xf2,	//频率偏移估计
	CC1101_REG_LQI			=	0xf3,	//连接质量的解调器估计
	CC1101_REG_RSSI			=	0xf4,	//接收信号强度指示
	CC1101_REG_MARCSTATE	=	0xf5,	//控制状态机状态
	CC1101_REG_WORTIME1		=	0xf6,	//WOR计时器高字节
	CC1101_REG_WORTIME0		=	0xf7,	//WOR计时器低字节
	CC1101_REG_PKTSTATUS	=	0xf8,	//当前GDOx状态和数据包状态
	CC1101_REG_VCOVCDAC		=	0xf9,	//PLL校准模块的当前设定
	CC1101_REG_TXBYTES		=	0xfA,	//TX FIFO中的下溢和比特数
	CC1101_REG_RXBYTES		=	0xfB,	//RX FIFO中的下溢和比特数
	//手册上面没有的
	CC1101_REG_STATUS1		=	0xfc,	//
	CC1101_REG_STATUS0		=	0xfd,	//
	//功率控制
	CC1101_REG_PATABLE0		=	0x40,
	CC1101_REG_PATABLE1		=	0x41,
	CC1101_REG_PATABLE2		=	0x42,
	CC1101_REG_PATABLE3		=	0x43,
	CC1101_REG_PATABLE4		=	0x44,
	CC1101_REG_PATABLE5		=	0x45,
	CC1101_REG_PATABLE6		=	0x46,
	CC1101_REG_PATABLE7		=	0x47,
}CC1101_REG_TYPE;
 

void Delay_ms(u16 time_ms);
u8 CC1101_ReadWriteByte(u8 data);
u8 CC1101_Command(CC1101_CMD_TYPE Cmd);
u8 CC1101_ReadReg(CC1101_REG_TYPE RegAddr);
u8 CC1101_WriteReg(CC1101_REG_TYPE RegAddr, u8 data);
void CC1101_Init(u8 Addr);
void CC1101_WriteTxFIFO(u8 *pBuff,u8 len);
void CC1101_ReadRxFIFO(u8 *pBuff,u8 len); 
void CC1101_RfDataSend(u8 *pBuff,u8 len);
void CC1101_RfDataSendPack(u8 *pBuff, u16 len);
u8 CC1101_GetStatus(void);


#endif    