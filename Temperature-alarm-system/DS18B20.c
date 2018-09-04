#include<reg51.h>

#define uchar unsigned char
#define uint unsigned int
sbit DQ = P2^2;//�¶ȴ������ӿ� 
bit     f=0;						  //�����¶ȵı�־λ,0����ʾ�����¶ȡ���1����ʾ�����¶ȡ���
bit     f_max=0;					  //�����¶ȵı�־λ��0����ʾ�����¶ȡ���1����ʾ�����¶ȡ���
bit     f_min=0;					  //�����¶ȵı�־λ��0����ʾ�����¶ȡ���1����ʾ�����¶ȡ���
extern uint wen_du;               //�¶ȱ���  
uchar max=0x00,min=0x00;		   //max�����ޱ����¶ȣ�min�����ޱ����¶�
uchar   temp=0; 			          //�����¶ȵ���������
uchar   temp_d=0; 		              //�����¶ȵ�С����

/***********ds18b20�ӳ��Ӻ���*******/
void delay_18B20(uint i)
{
	while(i--);
}
/**********ds18b20��ʼ������**********************/
void Init_DS18B20()
{
	uchar x=0;
	DQ=1;          //DQ��λ
	delay_18B20(8);  //������ʱ
	DQ=0;          //��Ƭ����DQ����
	delay_18B20(80); //��ȷ��ʱ ���� 480us
	DQ=1;          //��������
	delay_18B20(14);
	x=DQ;            //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	delay_18B20(20);
}
/***********ds18b20��һ���ֽ�**************/  
uchar ReadOneChar()
{
	uchar i;
	uchar dat=0;
	for (i=8;i>0;i--)
	{
		DQ=0; // �������ź�
		dat>>=1;
		DQ=1; // �������ź�
		if(DQ)
		{
			dat|=0x80;
		}
		delay_18B20(4);
	}
	return(dat);
}
/*************ds18b20дһ���ֽ�****************/  
void WriteOneChar(uchar dat)
{
	uchar i;
	for (i=8;i>0;i--)
	{
		DQ=0;
		DQ=dat&0x01;
		delay_18B20(5);
		DQ=1;
		dat>>=1;
	}
}
/**************��ȡds18b20��ǰ�¶�************/
void ReadTemperature()
{
	float tt;
	uchar a=0,b=0;
	Init_DS18B20();
	WriteOneChar(0xCC);         // ����������кŵĲ���
	WriteOneChar(0x44);         // �����¶�ת��
	delay_18B20(100);       
	Init_DS18B20();
	WriteOneChar(0xCC);         //����������кŵĲ���
	WriteOneChar(0xBE);         //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	delay_18B20(100);
	a=ReadOneChar();            //��ȡ�¶�ֵ��λ
	b=ReadOneChar();            //��ȡ�¶�ֵ��λ
	wen_du=b;
  	wen_du<<=8;             //two byte  compose a int variable
  	wen_du=wen_du|a;
  	tt=wen_du*0.0625;
  	wen_du=tt*10+0.5;
	//wen_du=((b*256+a)>>4);    //��ǰ�ɼ��¶�ֵ��16��ʵ���¶�ֵ
	/*wen_du=b;
	wen_du<<=8;             
	wen_du=wen_du|a;
	tt=wen_du*0.0625;
	wen_du=tt*10+0.5; */
}