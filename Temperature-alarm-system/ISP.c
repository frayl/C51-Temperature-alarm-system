#include<reg51.h>
#include <intrins.h>

sfr ISP_DATA =  0xe2; //Flash���ݼĴ�����ַ��E2h��
sfr ISP_ADDRH = 0xe3;//Flash���ֽڼĴ�����ַ��E3h��
sfr ISP_ADDRL = 0xe4;//Flash���ֽڼĴ�����ַ��E4h��
sfr ISP_CMD =   0xe5;  //Flash����ģʽ�Ĵ�����ַ��E5h��
sfr ISP_TRIG =  0xe6; //Flash������Ĵ�����ַ��E6h��
sfr ISP_CONTR = 0xe7;//Flash���ƼĴ�����ַ��E7h��
#define uchar unsigned char
#define WaitTime 0x81 //����CPU�ĵȴ�ʱ��  ��ISPEN=1ʱ,��ISP_TRIG ��������Ĵ�������д��46h����д��B9h��ISP/IAP����Ż���Ч��
#define RdCmd 0x01 //����ISP�Ĳ������� 
#define PrgCmd 0x02 
#define EraseCmd 0x03

void ISP_IAP_enable(void);                  /* ================ �� ISP,IAP ���� ================= */
void ISP_IAP_disable(void);                 /* =============== �ر� ISP,IAP ���� ================== */ 
void ISP_trig(void);                        /* ================ ���õĴ������� ==================== */
uchar byte_read(unsigned int byte_addr);    /* ============ �ֽڶ� ============= */ 
void SectorErase(unsigned int sector_addr); /* =========== �������� ============ */ 
void byte_write(unsigned int byte_addr, unsigned char dat); /* ==== �ֽ�д ==== */

void ISP_IAP_enable(void) /*================ �� ISP,IAP ���� =================*/ 
{
	EA = 0;                             //���ж�
	ISP_CONTR = ISP_CONTR & 0x18;       //ISP/IAP���ƼĴ�����λ 
	ISP_CONTR = ISP_CONTR | WaitTime;   //(10000001) ISPEN = 1д��Ӳ����ʱ
} 
void ISP_IAP_disable(void) /*=============== �ر� ISP,IAP ���� ==================*/ 
{ 
	ISP_CONTR = ISP_CONTR & 0x7f;       //(01111111)ISPEN = 0
	ISP_TRIG = 0x00; 					//���ISP_TRIG�Ĵ���
	EA = 1;                             //���ж�
} 
void ISP_trig(void) /*================�������� ====================*/ 
{ 
	ISP_IAP_enable(); 
	ISP_TRIG = 0x46; //����ISP_IAP������ֽ�ISP_ADDRHд��     ��ISP_TRIG��д��46h��
	ISP_TRIG = 0xb9; //����ISP_IAP������ֽ�ISP_ADDRLд��     ��д��B9h��ISP/IAP����Ż���Ч�� 
	_nop_(); 
} 
unsigned char byte_read(unsigned int byte_addr) /*========= �ֽڶ� =============*/ 
{ 
	ISP_ADDRH = (unsigned char)(byte_addr >> 8);     //����byte_addr�ĸ߰�λ
	ISP_ADDRL = (unsigned char)(byte_addr & 0x00ff); //����byte_addr�ĵͰ�λ
	ISP_CMD   = ISP_CMD & 0xf8;   //(11111000)�����3λ 
	ISP_CMD   = ISP_CMD | RdCmd;  //д�������  
	ISP_trig();                   //����ִ�� 
	ISP_IAP_disable();            //�ر�ISP,IAP���� 
	return (ISP_DATA);            //���ض��������� 
} 
void SectorErase(unsigned int sector_addr) /*=========== �������� ============*/ 
{ 
	unsigned int iSectorAddr; 
	iSectorAddr = (sector_addr & 0xfe00); //ȡ������ַ 
	ISP_ADDRH = (unsigned char)(iSectorAddr >> 8); 
	ISP_ADDRL = 0x00; 
	ISP_CMD = ISP_CMD & 0xf8;     //(11111000)�����3λ
	ISP_CMD = ISP_CMD | EraseCmd; //д�������������� 
	ISP_trig();       			  //����ִ�� 
	ISP_IAP_disable();            //�ر�ISP,IAP����
} 
void byte_write(unsigned int byte_addr, unsigned char dat) /*==== �ֽ�д ====*/ 
{ 
	ISP_ADDRH = (unsigned char)(byte_addr >> 8);     //����byte_addr�ĸ߰�λ
	ISP_ADDRL = (unsigned char)(byte_addr & 0x00ff); //����byte_addr�ĵͰ�λ
	ISP_CMD  = ISP_CMD & 0xf8;    //(11111000)�����3λ
	ISP_CMD  = ISP_CMD | PrgCmd;  //д��д���� 
	ISP_DATA = dat;               //д������׼�� 
	ISP_trig();                   //����ִ�� 
	ISP_IAP_disable();            //�ر�ISP,IAP���� 
}