#ifndef __USER_H__
#define __USER_H__



#define u8   unsigned char	
#define u16  unsigned int
#define u32  unsigned long
#define i8   unsigned short
#define i16  unsigned int





#define  Buzzer            PD_ODR_ODR3   //������
#define  Button            PC_IDR_IDR4   //��ť����
#define  CC1101_GDO0       PD_IDR_IDR2   //PD2
#define  CC1101_GDO2       PC_IDR_IDR3   //PC3




//SPI�ӿ�  
//�ײ�ӿں궨��  
#define CC1101_CS_H()                PA_ODR_ODR3=1           //PA3=1  
#define CC1101_CS_L()                PA_ODR_ODR3=0           //PA3=0  
#define CC1101_MOSI_H()              PC_ODR_ODR6=1           //PC6  
#define CC1101_MOSI_L()              PC_ODR_ODR6=0           //PC6  
#define CC1101_SCLK_H()              PC_ODR_ODR5=1           //PC5  
#define CC1101_SCLK_L()              PC_ODR_ODR5=0           //PC5  
#define CC1101_GetMISO               PC_IDR_IDR7              //PC7  
  


//CC1101 ����
//��д�ķ�ʽ��ֱ�ӷ��ʽ�������Ӧ������
typedef enum
{
	CC1101_CMD_SRES		=	0x30,	//����
	CC1101_CMD_SFSTXON	=	0x31,	//������У׼Ƶ�ʺϳ�������MCSM0.FSAUTOCAL=1��
	CC1101_CMD_SXOFF	=	0x32,	//�رվ�������
	CC1101_CMD_SCAL		=	0x33,	//У׼Ƶ�ʺϳ������ضϣ������������������ڲ������ֶ�У׼ģʽ��MCSM0.FS_AUTOCAL=0��������£�SCAL�ܴӿ���ģʽ�˲���
	CC1101_CMD_SRX		=	0x34,	//����RX������һ״̬Ϊ������MCSM0.FS_AUTOCAL=1����������У׼��
	CC1101_CMD_STX		=	0x35,	//����״̬������TX����MCSM0.FS_AUTOCAL=1��������У׼������RX״̬��CCA���ã����ŵ�Ϊ�������TX
	CC1101_CMD_SIDLE	=	0x36,	//�뿪RX/TX,�ض�Ƶ�ʺϳ������뿪��Ų�����ģʽ������
	CC1101_CMD_SAFC		=	0x37,	//����22.1���г���Ƶ�ʺϳ�����AFC����
	CC1101_CMD_SWOR		=	0x38,	//����27.5���������Զ�RXѡ�����У���Ų����
	CC1101_CMD_SPWD		=	0x39,	//��CSnΪ��ʱ���빦�ʽ���ģʽ��
	CC1101_CMD_SFRX		=	0x3a,	//��ϴRX FIFO����
	CC1101_CMD_SFTX		=	0x3b,	//��ϴTX FIFO����
	CC1101_CMD_SWORRST	=	0x3c,	//����������ʵʱ��ʱ��
	CC1101_CMD_SNOP		=	0x3d,	//�޲�������������Ϊ���򵥵�������˲������Ϊ2�ֽڡ�
}CC1101_CMD_TYPE;
 
 
 
//CC1101�Ĵ�������
typedef enum
{
	//�ɶ�д�ļĴ���
	CC1101_REG_IOCFG2		=	0x00,	//GDO2���������
	CC1101_REG_IOCFG1		=	0x01,	//GDO1���������
	CC1101_REG_IOCFG0		=	0x02,	//GDO0���������
	CC1101_REG_FIFOTHR		=	0x03,	//RX FIFO��TX FIFO����
	CC1101_REG_SYNC1		=	0x04,	//ͬ���ʻ㣬���ֽ�
	CC1101_REG_SYNC0		=	0x05,	//ͬ���ʻ㣬���ֽ�
	CC1101_REG_PKTLEN		=	0x06,	//���ݰ�����
	CC1101_REG_PKTCTRL1		=	0x07,	//���ݰ��Զ�����
	CC1101_REG_PKTCTRL0		=	0x08,	//���ݰ��Զ�����
	CC1101_REG_ADDR			=	0x09,	//�豸��ַ
	CC1101_REG_CHANNR		=	0x0a,	//�ŵ���
	CC1101_REG_FSCTRL1		=	0x0b,	//Ƶ�ʺϳ������ƣ����ֽ�
	CC1101_REG_FSCTRL0		=	0x0c,	//Ƶ�ʺϳ������ƣ����ֽ�
	CC1101_REG_FREQ2		=	0x0d,	//Ƶ�ʿ��ƴʻ㣬���ֽ�
	CC1101_REG_FREQ1		=	0x0e,	//Ƶ�ʿ��ƴʻ㣬�м��ֽ�
	CC1101_REG_FREQ0		=	0x0f,	//Ƶ�ʿ��ƴʻ㣬���ֽ�
	CC1101_REG_MDMCFG4		=	0x10,	//����������
	CC1101_REG_MDMCFG3		=	0x11,	//����������
	CC1101_REG_MDMCFG2		=	0x12,	//����������
	CC1101_REG_MDMCFG1		=	0x13,	//����������
	CC1101_REG_MDMCFG0		=	0x14,	//����������
	CC1101_REG_DEVIATN		=	0x15,	//��������������
	CC1101_REG_MCSM2		=	0x16,	//��ͨ�ſ���״̬������
	CC1101_REG_MCSM1		=	0x17,	//��ͨ�ſ���״̬������
	CC1101_REG_MCSM0		=	0x18,	//��ͨ�ſ���״̬������
	CC1101_REG_FOCCFG		=	0x19,	//Ƶ��ƫ�Ʋ�������
	CC1101_REG_BSCFG		=	0x1a,	//λͬ������
	CC1101_REG_AGCTRL2		=	0x1b,	//AGC����
	CC1101_REG_AGCTRL1		=	0x1c,	//AGC����
	CC1101_REG_AGCTRL0		=	0x1d,	//AGC����
	CC1101_REG_WOREVT1		=	0x1e,	//���ֽ�ʱ��0��ͣ
	CC1101_REG_WOREVT0		=	0x1f,	//���ֽ�ʱ��0��ͣ
	CC1101_REG_WORCTRL		=	0x20,	//��Ų��������
	CC1101_REG_FREND1		=	0x21,	//ǰĩ��RX����
	CC1101_REG_FREND0		=	0x22,	//ǰĩ��TX����
	CC1101_REG_FSCAL3		=	0x23,	//Ƶ�ʺϳ���У׼
	CC1101_REG_FSCAL2		=	0x24,	//Ƶ�ʺϳ���У׼
	CC1101_REG_FSCAL1		=	0x25,	//Ƶ�ʺϳ���У׼
	CC1101_REG_FSCAL0		=	0x26,	//Ƶ�ʺϳ���У׼
	CC1101_REG_RCCTRL1		=	0x27,	//RC��������
	CC1101_REG_RCCTRL0		=	0x28,	//RC��������
	CC1101_REG_FSTEST		=	0x29,	//Ƶ�ʺϳ���У׼����
	CC1101_REG_PTEST		=	0x2a,	//��Ʒ����
	CC1101_REG_AGCTEST		=	0x2b,	//AGC����
	CC1101_REG_TEST2		=	0x2c,	//��ͬ�Ĳ�������
	CC1101_REG_TEST1		=	0x2d,	//��ͬ�Ĳ�������
	CC1101_REG_TEST0		=	0x2e,	//��ͬ�Ĳ�������
	//ֻ����״̬�Ĵ���,���д�뽫���������˲�
	CC1101_REG_PARTNUM		=	0xf0,	//CC2550����ɲ�����Ŀ
	CC1101_REG_VERSION		=	0xf1,	//��ǰ�汾��
	CC1101_REG_FREQEST		=	0xf2,	//Ƶ��ƫ�ƹ���
	CC1101_REG_LQI			=	0xf3,	//���������Ľ��������
	CC1101_REG_RSSI			=	0xf4,	//�����ź�ǿ��ָʾ
	CC1101_REG_MARCSTATE	=	0xf5,	//����״̬��״̬
	CC1101_REG_WORTIME1		=	0xf6,	//WOR��ʱ�����ֽ�
	CC1101_REG_WORTIME0		=	0xf7,	//WOR��ʱ�����ֽ�
	CC1101_REG_PKTSTATUS	=	0xf8,	//��ǰGDOx״̬�����ݰ�״̬
	CC1101_REG_VCOVCDAC		=	0xf9,	//PLLУ׼ģ��ĵ�ǰ�趨
	CC1101_REG_TXBYTES		=	0xfA,	//TX FIFO�е�����ͱ�����
	CC1101_REG_RXBYTES		=	0xfB,	//RX FIFO�е�����ͱ�����
	//�ֲ�����û�е�
	CC1101_REG_STATUS1		=	0xfc,	//
	CC1101_REG_STATUS0		=	0xfd,	//
	//���ʿ���
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