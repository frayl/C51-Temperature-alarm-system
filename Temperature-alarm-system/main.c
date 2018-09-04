#include<reg51.h>
#include<math.h>
#define uchar unsigned char
#define uint unsigned int

sbit LEDRED = P1^0;
sbit LEDBLUE = P1^3;
sbit buzz = P2^3;//���������ƶ˽ӿ�
sbit key1 = P3^0;//�����¶�
sbit key2 = P3^1;//�¶ȼ�
sbit key3 = P3^2;//�¶ȼ�
sbit dula = P2^6;//����ܶ�ѡ
sbit wela = P2^7;//�����λѡ
/////��������ܶ�ѡ//////////////////////////////////////////////
uchar table[22]=
{0x3F,0x06,0x5B,0x4F,0x66,		 //	0,1,2,3,4
0x6D,0x7D,0x07,0x7F,0x6F,		 //	5,6,7,8,9
0x77,0x7C,0x39,0x5E,0x79,0x71,	 //	A,B,C,D,E,F
0x40,0x38,0x76,0x00,0xff,0x37};//'-',L,H,��,ȫ����n    16-21
unsigned char code table1[]=
{0xBF,0x86,0xDB,0xCF,0xE6,
0xED,0xFD,0x87,0xFF,0xEF};
 
uchar byte_read(unsigned int byte_addr);     //�ֽڶ� 
void SectorErase(unsigned int sector_addr);  //�������� 
void byte_write(unsigned int byte_addr,char dat);  //�ֽ�д
extern void ReadTemperature();
int wen_du;     //�¶ȱ���  
int high,low;   //�Ա��¶��ݴ����
uchar flag,a_a;	//��ǰ����״̬��־
uchar d1,d2,d2t,d3,d4; //��ʾ�����ݴ����

void delay(uint ms)//��ʱ��������Լ��ʱ25us
{
	uchar x;
	for(ms;ms>0;ms--)
	{
		for(x=10;x>0;x--);
	}
}

void display()//��ʾʵʱ�¶�
{       
	/*dula = 0;
	P0 = table[d1];
	delay(10);//��1λ
	dula = 1;
	dula = 0;
	wela = 0;
	P0 = 0xfe;
	wela = 1;
	wela = 0;
	delay(1);*/
			  
	dula=0;
	P0=table[d1];
	delay(10);//��2λ
	dula=1;
	dula=0;
	wela=0;
	P0=0xfd;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table1[d2];
	delay(10);//��3λ
	dula=1;
	dula=0;
	wela=0;
	P0=0xfb;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table[d3];
	delay(10);//��4λ
	dula=1;
	dula=0;
	wela=0;
	P0=0xf7;
	wela=1;
	wela=0;
	delay(1);
}
void display1()//��ʾ�����¶�  100-125	 -55-(-1)
{       
	dula = 0;
	P0 = table[d1];
	delay(10);//��1λ
	dula = 1;
	dula = 0;
	wela = 0;
	P0 = 0xfe;
	wela = 1;
	wela = 0;
	delay(1);
			  
	dula=0;
	P0=table[d2];
	delay(10);//��2λ
	dula=1;
	dula=0;
	wela=0;
	P0=0xfd;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table[d3];
	delay(10);//��3λ
	dula=1;
	dula=0;
	wela=0;
	P0=0xfb;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table[d4];
	delay(10);//��4λ
	dula=1;
	dula=0;
	wela=0;
	P0=0xf7;
	wela=1;
	wela=0;
	delay(1);
}
void display2()//��ʾ�����¶�  0-99
{       
	dula = 0;
	P0 = table[d1];
	delay(10);//��1λ
	dula = 1;
	dula = 0;
	wela = 0;
	P0 = 0xfe;
	wela = 1;
	wela = 0;
	delay(1);
	
	dula=0;
	P0=table[d2];
	delay(10);//��3λ
	dula=1;
	dula=0;
	wela=0;
	P0=0xfb;
	wela=1;
	wela=0;
	delay(1);
	
	dula=0;
	P0=table[d3];
	delay(10);//��4λ
	dula=1;
	dula=0;
	wela=0;
	P0=0xf7;
	wela=1;
	wela=0;
	delay(1);
}

void keyscan()//����ɨ�躯��
{
	int i;
	if(key1==0)              //���ü�����
    {
    	delay(300);          //��ʱȥ��
        if(key1==0)
		{
			flag=1;            //�ٴ��жϰ��������µĻ���������״̬
        }
		while(key1==0);         //���ּ��                  
    }
    while(flag==1)              //������������״̬
    {
    	d1=18;				 //��ʾ��ĸH
		if(high>=100)
		{	
			d2=1;		     //����ܵ�2λ��ʾ����1  
			d3=high%100/10;
			d4=high%100%10;     //�����¶�ֵ
        	display1();		   //������ʾ����
		}
		else if(high<100&&high>=0)
		{
			d2=high/10;
			d3=high%10;			 //�����¶�ֵ
			display2();			//������ʾ����
		}
		else
		{
			i=abs(high);		//ȡ����ֵ
			d2=16;				//����ܵ�2λ��ʾ����-
			d3=i/10;
			d4=i%10;            //�����¶�ֵ
        	display1();		   //������ʾ����
		}              
        if(key1==0)             //�ж����ü��Ƿ���
        {
        	delay(300);         //��ʱȥ��
            if(key1==0)
			{
				flag=2;         //�������£�������������ģʽ
            }
			while(key1==0);     //���ּ��
        }
        if(key2==0)             //�Ӽ�����
        {
        	delay(300);         //��ʱȥ��
            if(key2==0)         //�Ӽ�����
            {
            	high+=1;        //���޼�1
                if(high>=125)
				{
					high=125;    //�������ӵ�125
                }
			}
			while(key2==0);     //���ּ��
        }
        if(key3==0)             //��������
        {
        	delay(300);          //��ʱȥ��
            if(key3==0)         //��������
            {
            	high-=1;        //���޼�1
                if(high<=-55)
				{
					high=-55;    //������С����-55
                }
			}
			while(key3==0);     //���ּ��
        }               
	}
    while(flag==2)              //��������
    {
    	d1=17;					//��ʾ��ĸL
		if(low>=100)
		{	
			d2=1;		     //����ܵ�2λ��ʾ����1  
			d3=low%100/10;
			d4=low%100%10;      //�����¶�ֵ
        	display1();		   //������ʾ����
		}
		else if(low<100&&low>=0)
		{
			d2=low/10;
			d3=low%10;		//�����¶�ֵ
			display2();		//������ʾ����
		}
		else
		{
			i=abs(low);		//ȡ����ֵ
			d2=16;
			d3=i/10;
			d4=i%10;        //�����¶�ֵ
        	display1();		//������ʾ����
		}
        if(key1==0)
        {
        	delay(300);
            if(key1==0)
			{
				flag=0;
			}
            while(key1==0);     //���ּ��
        	SectorErase(0x2e00);//������������
	    	byte_write(0x2e00,high%256);//��high���¶����ޣ�д������
	    	byte_write(0x2e01,high/256);
	                
	   		byte_write(0x2e20,low%256); //��low���¶����ޣ�д������
	    	byte_write(0x2e21,low/256); 
	                         
	    	byte_write(0x2e55,a_a);  //��a_a��ֵд������
		}
        if(key2==0)
        {
        	delay(300);
            if(key2==0)
            {
            	low+=1;
                if(low>=125)
				{
					low=125;
				}       
            }
			while(key2==0);      //���ּ��
        }
        if(key3==0)
        {
        	delay(300);
            if(key3==0)
            {
            	low-=1;
                if(low<=-55)
				{
					low=-55;
				}       
            }
			while(key3==0);//���ּ��
        }		               
	}
		
}
void zi_dong()//�Զ��¿�ģʽ
{
	d1=wen_du/100;
   	d2t=wen_du%100;
   	d2=d2t/10;
   	d3=d2t%10;
    keyscan();		//����ɨ�躯��
    display();		//������ʾ����
    if((wen_du/10)<=low)	//���¾���
	{
		//buzz=0;
		LEDBLUE = 0;
	}			//�������� ��������������
    if(((wen_du/10)>low)&&((wen_du/10)<high))//�¶ȴ������ޣ�С������ 
    {                                                                                        
        buzz = 1;
		P1 = 0xff;                   
    }
    if((wen_du/10)>=high)	 //���¾���
	{
		//buzz = 0;
		LEDRED = 0;
	}			//�������� �������������
}
void main()                  //������
{	
	uint j;
	high = byte_read(0x2e01);//����ʼʱ��ȡEEPROM�е����ݣ���ȡ���ޣ� 
    high <<= 8;
    high |= byte_read(0x2e00);
    if(high>200)
	{
		high = high - 256;
	}
	low = byte_read(0x2e21);//        ����ʼʱ��ȡEEPROM�е����ݣ���ȡ���ޣ�        
    low <<= 8;
    low |= byte_read(0x2e20);
    if(low>200)
	{
		low = low - 256;
	}
	a_a = byte_read(0x2e55);//         
    if(a_a!=22) //��ֹ�״��ϵ�ʱ��ȡ���� 
    {
    	high=30;        //��һ���ϵ��¶�����Ϊ30.C    
        low=20;         //��һ���ϵ��¶�����Ϊ20.C
    	a_a=22;
   	}          
	delay(150);	
    for(j=0;j<80;j++)          //�ȶ�ȡ�¶�ֵ����ֹ������ʾ85
    {
		ReadTemperature();
    }
	while(1)                          //����whileѭ��
    {       
    	ReadTemperature();        //��ȡ�¶�ֵ
        for(j=0;j<100;j++)
		{
			zi_dong();//�¿�ģʽ
		}
    }
}
