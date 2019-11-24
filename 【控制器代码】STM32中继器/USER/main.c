#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "led.h"
#include "JVTWB.h"

//EP1����
//EP2����
extern u8 HID_Buf[8];
u16 DownloadData[2048];
u16 DownloadDataidx;
u16 DownloadAddr;
u8 rec_data=0;
 int main(void)
 {		
	u16 i;
	u16 Data;
	int timecnt=0;

	
	u8 HID_TXbuf[8]={0xFA,0x01,0,0,0,0,0,0xFA};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ͷ�PB3 PB4
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	LED_Init();
	JVTWB_Init();

	USB_Port_Set(0); 	//�ϵ�
	delay_ms(300);
  USB_Port_Set(1);	//�ϵ�
	//USB����
 	USB_Interrupts_Config();    
	delay_ms(300);
 	Set_USBClock();   
	delay_ms(300);
 	USB_Init();	        
	printf("USB��ʼ�����\r\n");
	 

	while(1)
	{

		if(rec_data)
		{			
			LED=~LED;
			Data=HID_Buf[2];
			Data=Data<<8|HID_Buf[3];
			
			if(HID_Buf[1]==0x01)
			{
				JVTWB_SrialSetData(Data);
			}
			else if(HID_Buf[1]==0x02)
			{
				JVTWB_ParallSetData(Data);
			}
			else if(HID_Buf[1]==0x03)//Flash���
			{
				if(HID_Buf[2]==0xAA)//��ʼ
				{
					DownloadDataidx=0;
					DownloadAddr=0;
				}
				else if(HID_Buf[2]==0xBB)
				{
						DownloadData[DownloadDataidx++]=HID_Buf[3]<<8|HID_Buf[4];
						DownloadData[DownloadDataidx++]=HID_Buf[5]<<8|HID_Buf[6];
				}					
				else if(HID_Buf[2]==0xCC)
				{
					int i;
					for(i=0;i<DownloadDataidx;i++)//���س��ȸ���DownloadDataidx��̬�仯
					{
						WriteAdata(DownloadAddr++,DownloadData[i]%256);
						WriteAdata(DownloadAddr++,DownloadData[i]/256);
					}
					//��������

				HID_TXbuf[1]=0x03;
				HID_TXbuf[2]=0xDD;
				UserToPMABufferCopy(HID_TXbuf,ENDP2_TXADDR,8);
				SetEPTxCount(2,8);
				SetEPTxValid(2);
				//�ϱ����سɹ�
				}
				
			}
			else if(HID_Buf[1]==0x04)//Flash��֤
			{
				//��ʼ����
				HID_TXbuf[1]=0x04;
				HID_TXbuf[2]=0xAA;
				UserToPMABufferCopy(HID_TXbuf,ENDP2_TXADDR,8);
				SetEPTxCount(2,8);
				SetEPTxValid(2);
				delay_ms(1);
				for(i=0;;)
				{
					HID_TXbuf[4]=ReadAdata(i++);
					HID_TXbuf[3]=ReadAdata(i++);
					
					HID_TXbuf[6]=ReadAdata(i++);
					HID_TXbuf[5]=ReadAdata(i++);
					
					HID_TXbuf[1]=0x04;
					HID_TXbuf[2]=0xBB;//������
					UserToPMABufferCopy(HID_TXbuf,ENDP2_TXADDR,8);
					
					
					SetEPTxCount(2,8);
					SetEPTxValid(2);
					delay_ms(10);
					if((HID_TXbuf[3]==0xFF&&HID_TXbuf[4]==0xFF)||(HID_TXbuf[5]==0xFF&&HID_TXbuf[6]==0xFF)) break;//����0xFF�Ͳ�����
				}
				delay_ms(1);
				//�������
				HID_TXbuf[1]=0x04;
				HID_TXbuf[2]=0xCC;
				UserToPMABufferCopy(HID_TXbuf,ENDP2_TXADDR,8);
				SetEPTxCount(2,8);
				SetEPTxValid(2);
				
			}
			else if(HID_Buf[1]==0x05)
			{
				
				//����
				ereaseSector(0);
				timecnt=0;
				while(RnB==0)
				{
					timecnt++;
					delay_ms(10);
					
					if(timecnt>=100) break;
				}
				if(timecnt>=100)//����ʧ��
				{
					HID_TXbuf[1]=0x02;
					HID_TXbuf[2]=0xFF;
				}
				else//�����ɹ�
				{
					HID_TXbuf[1]=0x02;
					HID_TXbuf[2]=0x00;
				}
				//�ϱ��������
				UserToPMABufferCopy(HID_TXbuf,ENDP2_TXADDR,8);
				SetEPTxCount(2,8);
				SetEPTxValid(2);
			}
			
			rec_data=0;
		}
		
	}
 }
