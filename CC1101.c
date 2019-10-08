/*************************************************************************************************************
     CC1101.C ����
*************************************************************************************************************/  

#include "IOSTM8S103F3.h"
#include "USER.h"   //�û����� 
  
//CC1101��������  
#define     WRITE_BURST         0x40                        //����д��  
#define     READ_SINGLE         0x80                        //��  
#define     WRITE_SINGLE        0x00                        //д  
#define     READ_BURST          0xC0  
  
  
  
//������  
#define     BURST_READ_FIFO     0xff        //ͻ����ȡRX FIFO  
#define     BYTE_READ_FIFO      0xBF        //���ֽڶ�ȡ RX FIFO  
#define     BURST_WRITE_FIFO    0x7f        //ͻ��дTX FIFO  
#define     BYTE_WRITE_FIFO     0x3f        //���ֽ�д TX FIFO  
  
#define     CC1101_DATA_LEN     64  
  
  
  

  
//CC1101 SPI��дһ�ֽ�  
//����Ƭѡ  
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
        CC1101_SCLK_H();                //ʱ��������д������  
        temp <<= 1;asm("NOP"); 
        if(CC1101_GetMISO) temp ++;  
        CC1101_SCLK_L();                //ʱ���½��ض�ȡ����  
    }  
  
    return temp;  
}  
  
  
/*************************************************************************************************************************
* ����    :   u8 CC1101_Command(CC1101_CMD_TYPE Cmd)
* ����    :   ���͵��ֽ�����
* ����    :   Cmd;����,��CC1101_CMD_TYPE
* ����    :   �Ĵ�����ֵ
* ����    :   �ײ�궨��
* ˵��    :   ��д�ķ�ʽ��ֱ�ӷ��ʽ�������Ӧ������
*************************************************************************************************************************/  
u8 CC1101_Command(CC1101_CMD_TYPE Cmd)  
{  
    u8 status;  
  
    CC1101_CS_L();                              //Ƭѡ��Ч  
    while(CC1101_GetMISO);  
    status = CC1101_ReadWriteByte((u8)Cmd);     //��������  
    while(CC1101_GetMISO);  
    CC1101_CS_H();                              //Ƭѡ�ر�  
    return status;  
}  
  
  
  
/*************************************************************************************************************************
* ����    :   u8 CC1101_ReadReg(CC1101_REG_TYPE RegAddr)
* ����    :   ��ȡCC1101ͨ�üĴ���
* ����    :   RegAddr:�Ĵ�����ַ,��:CC1101_REG_TYPE
* ����    :   �Ĵ�����ֵ
* ����    :   �ײ�궨��
* ˵��    :   һ�ζ�ȡһ���Ĵ���
*************************************************************************************************************************/  
u8 CC1101_ReadReg(CC1101_REG_TYPE RegAddr)  
{  
    u8 data;  
  
    CC1101_CS_L();                                  //Ƭѡ��Ч  
    CC1101_ReadWriteByte((u8)READ_SINGLE|RegAddr);  //���Ͷ������Լ��Ĵ�������  
    data = CC1101_ReadWriteByte(0xff);              //��ȡ  
    CC1101_CS_H();                                  //Ƭѡ�ر�  
    return data;  
}  
  
  
/*************************************************************************************************************************
* ����    :   u8 CC1101_WriteReg(CC1101_REG_TYPE RegAddr, u8 data)
* ����    :   д��CC1101ͨ�üĴ���
* ����    :   RegAddr:�Ĵ�����ַ,��:CC1101_REG_TYPE,data:��Ҫд�������
* ����    :   ״̬�Ĵ�����ֵ
* ����    :   �ײ�궨��
* ˵��    :   һ��д��һ���Ĵ���,������״̬
            ��Ҫ��ֻ���ļĴ�������д����
*************************************************************************************************************************/  
u8 CC1101_WriteReg(CC1101_REG_TYPE RegAddr, u8 data)  
{  
    u8 status;  
  
    if(RegAddr > 0x80) return 0;                             //��ֹ�����,0x30�Ժ�ļĴ���Ϊֻ��״̬�Ĵ���  
    CC1101_CS_L();                                              //Ƭѡ��Ч  
    while(CC1101_GetMISO);  
    status = CC1101_ReadWriteByte((u8)WRITE_SINGLE|RegAddr);    //����д�����Լ��Ĵ�������  
    CC1101_ReadWriteByte(data);                                 //д������  
    CC1101_CS_H();                                              //Ƭѡ�ر�  
    return status;  
}  
  
  

void CC1101_Init(u8 Addr)  
{  
  
    //��ʼ���Ĵ���  
    CC1101_Command(CC1101_CMD_SRES);                    //��λ  
    //Delay_ms(10);  
      
    while(CC1101_ReadReg(CC1101_REG_AGCTEST) != 0x3F)   //���ͨ��  
    {  
        //LED_ON();  
        //Delay_MS(10);  
        //LED_OFF();  
        //Delay_MS(100);  
    }  
    //LED_OFF();  
  
    CC1101_WriteReg(CC1101_REG_IOCFG0,0x06);            //������ʾ����  
    CC1101_WriteReg(CC1101_REG_IOCFG2,0x01);            //������ʾ����  
  
    CC1101_WriteReg(CC1101_REG_FIFOTHR,0x0f);           //RX FIFO��TX FIFO����  
    CC1101_WriteReg(CC1101_REG_SYNC1,0xD3);             //ͬ���ʻ㣬���ֽ�  
    CC1101_WriteReg(CC1101_REG_SYNC0,0x91);             //ͬ���ʻ㣬���ֽ�  
    CC1101_WriteReg(CC1101_REG_PKTLEN,CC1101_DATA_LEN); //���ݰ�����,
    CC1101_WriteReg(CC1101_REG_PKTCTRL1,0x04);          //���ݰ��Զ�����  
    CC1101_WriteReg(CC1101_REG_PKTCTRL0,0x04);          //���ݰ��Զ�����  
    CC1101_WriteReg(CC1101_REG_ADDR,0x00);              //�豸��ַ  
    CC1101_WriteReg(CC1101_REG_CHANNR,0x00);            //�ŵ�  
    CC1101_WriteReg(CC1101_REG_FSCTRL1,0x06);           //Ƶ�ʺϳ������ƣ����ֽ�  
    CC1101_WriteReg(CC1101_REG_FSCTRL0,0x00);           //Ƶ�ʺϳ������ƣ����ֽ�  
    CC1101_WriteReg(CC1101_REG_FREQ2,0x10);             //Ƶ�ʿ��ƴʻ㣬���ֽ�  
    CC1101_WriteReg(CC1101_REG_FREQ1,0xb1);             //Ƶ�ʿ��ƴʻ㣬�м��ֽ�  
    CC1101_WriteReg(CC1101_REG_FREQ0,0x3b);             //Ƶ�ʿ��ƴʻ㣬���ֽ�  
      
    //2.4KBPS  
    CC1101_WriteReg(CC1101_REG_MDMCFG4,0xF6);           //����������  
    CC1101_WriteReg(CC1101_REG_MDMCFG3,0x83);           //����������  
      
    CC1101_WriteReg(CC1101_REG_MDMCFG2,0x13);           //����������  
    CC1101_WriteReg(CC1101_REG_MDMCFG1,0x22);           //����������  
    CC1101_WriteReg(CC1101_REG_MDMCFG0,0xf8);           //����������  
      
    CC1101_WriteReg(CC1101_REG_DEVIATN,0x15);           //��������������  
    CC1101_WriteReg(CC1101_REG_MCSM2,0x07);             //��ͨ�ſ���״̬������  
    CC1101_WriteReg(CC1101_REG_MCSM1,0x30);             //��ͨ�ſ���״̬������  
    CC1101_WriteReg(CC1101_REG_MCSM0,0x18);             //��ͨ�ſ���״̬������  
    CC1101_WriteReg(CC1101_REG_FOCCFG,0x16);            //Ƶ��ƫ�Ʋ�������  
    CC1101_WriteReg(CC1101_REG_BSCFG,0x6c);             //λͬ������  
    CC1101_WriteReg(CC1101_REG_AGCTRL2,0x03);           //AGC����  
    CC1101_WriteReg(CC1101_REG_AGCTRL1,0x40);           //AGC����  
    CC1101_WriteReg(CC1101_REG_AGCTRL0,0x91);           //AGC����  
    CC1101_WriteReg(CC1101_REG_WOREVT1,0x87);           //���ֽ�ʱ��0��ͣ  
    CC1101_WriteReg(CC1101_REG_WOREVT0,0x6b);           //���ֽ�ʱ��0��ͣ  
    CC1101_WriteReg(CC1101_REG_WORCTRL,0xfb);           //��Ų��������  
    CC1101_WriteReg(CC1101_REG_FREND1,0x56);            //ǰĩ��RX����  
    CC1101_WriteReg(CC1101_REG_FREND0,0x10);            //ǰĩ��TX����  
    CC1101_WriteReg(CC1101_REG_FSCAL3,0xe9);            //Ƶ�ʺϳ���У׼  
    CC1101_WriteReg(CC1101_REG_FSCAL2,0x2a);            //Ƶ�ʺϳ���У׼  
    CC1101_WriteReg(CC1101_REG_FSCAL1,0x00);            //Ƶ�ʺϳ���У׼  
    CC1101_WriteReg(CC1101_REG_FSCAL0,0x1f);            //Ƶ�ʺϳ���У׼  
    CC1101_WriteReg(CC1101_REG_RCCTRL1,0x41);           //RC��������  
    CC1101_WriteReg(CC1101_REG_RCCTRL0,0x00);           //RC��������  
    CC1101_WriteReg(CC1101_REG_FSTEST,0x59);            //Ƶ�ʺϳ���У׼����  
      
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
* ����    :   void CC1101_WriteTxFIFO(u8 *pBuff,u8 len)
* ����    :   д�����ݵ����ͻ�����
* ����    :   pBuff:��Ҫд������ݻ�����ָ��,len:��Ҫд������ݳ���
* ����    :   ��
* ����    :   �ײ�궨��
* ˵��    :   д�����ݵ�����FIFO
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
* ����    :   void CC1101_ReadRxFIFO(u8 *pBuff,u8 len)
* ����    :   ��ȡ���ջ�����
* ����    :   pBuff:��Ҫ��ȡ�����ݻ�����ָ��,len:��Ҫ��ȡ�����ݳ���
* ����    :   ��
* ����    :   �ײ�궨��
* ˵��    :   �ӽ���FIFO��ȡ����
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
  
  
//��������,������������ȫ�����ͳ�ȥ  
//һ�����64B,��Ϊ�ܵ�FIFO����  
void CC1101_RfDataSend(u8 *pBuff,u8 len)  
{  
    CC1101_Command(CC1101_CMD_SIDLE);   //�˳���ǰģʽ  
    CC1101_Command(CC1101_CMD_SFTX);    //��շ��ͻ�����  
    CC1101_WriteTxFIFO(pBuff, len);     //д�����ݵ����ͻ�����  
    CC1101_Command(CC1101_CMD_STX);     //��ʼ��������  
      
    while(!CC1101_GDO0);  
    while(CC1101_GDO0);  
  
    CC1101_Command(CC1101_CMD_SIDLE);   //�˳���ǰģʽ  
}  
  
  
  
  
  
//�������ݰ�  
//ÿ�η������65B,��һ�ֽ�Ϊ����,���ݶཫ���ظ�����  
//���Է��������С  
//CC1101PackSize��Ч���ݰ���С,0-64,Ҳ����CC1101���η������ݴ�С-1  
void CC1101_RfDataSendPack(u8 *pBuff, u16 len)  
{  
    u16 i,m,n,j;  
      
    m = len / (CC1101_DATA_LEN-1);          //��������֡����  
    n = len % (CC1101_DATA_LEN-1);          //����  
      
    //����������  
    for(i = 0;i < m;i ++)                  
    {  
        Delay_ms(1);  
        CC1101_Command(CC1101_CMD_SIDLE);   //�˳���ǰģʽ  
        CC1101_Command(CC1101_CMD_SFTX);    //��շ��ͻ�����  
         
        CC1101_CS_L();  
        CC1101_ReadWriteByte(BURST_WRITE_FIFO);  
         
        CC1101_ReadWriteByte(CC1101_DATA_LEN-1);//��д�����С      
        for(j = 0;j < (CC1101_DATA_LEN-1);j ++)  
        {  
            CC1101_ReadWriteByte(*pBuff++); //д�����ݵ����ͻ�����  
        }  
        CC1101_CS_H();  
         
        CC1101_Command(CC1101_CMD_STX);     //��ʼ��������  
        while(!CC1101_GDO0);  
        while(CC1101_GDO0);                 //�ȴ��������  
    }  
    //����������  
    if(n!=0)  
    {  
        Delay_ms(1);  
        CC1101_Command(CC1101_CMD_SIDLE);   //�˳���ǰģʽ  
        CC1101_Command(CC1101_CMD_SFTX);    //��շ��ͻ�����  
        CC1101_CS_L();  
        CC1101_ReadWriteByte(BURST_WRITE_FIFO);  
         
        CC1101_ReadWriteByte(n);            //��д�����С      
        for(j = 0;j < n;j ++)  
        {  
            CC1101_ReadWriteByte(*pBuff++); //д�����ݵ����ͻ�����  
        }  
        CC1101_CS_H();  
         
        CC1101_Command(CC1101_CMD_STX);     //��ʼ��������  
        while(!CC1101_GDO0);  
        while(CC1101_GDO0);                 //�ȴ��������  
    }  
    CC1101_Command(CC1101_CMD_SIDLE);   //�˳���ǰģʽ  
}  
  
  
  
  
  
  
  
//��ȡоƬ״̬  
u8 CC1101_GetStatus(void)  
{  
    return CC1101_WriteReg(CC1101_REG_TEST2, 0x98);  
}