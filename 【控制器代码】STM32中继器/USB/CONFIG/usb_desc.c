/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptors for Relay Mouse Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"


//�豸����
const uint8_t Relay_DeviceDescriptor[RELAY_SIZ_DEVICE_DESC] =
  {
    0x12,                //����������С
    0x01,								//�豸������
    0x00, 0x02,      //USB�汾��  USB2.0
    0x00,                //bDeviceClass       �豸����� ��USB�ٷ���
    0x00,                //bDeviceSubClass  ������루USB�ٷ���
    0x00,                //bDeviceProtocol   �豸Э����루USB�ٷ���
    0x40,              	//�˵�0�������Ĵ�С
    0xBB,0xAA,       //�������̱�ţ�USB�ٷ���
   
    0x01,0x00,       //��Ʒ��ţ������̷��䣩

    0x01,0x00,        //�豸�������
 
    1,                      //�豸�����ַ�������
                                                 
    2,                      //��Ʒ�����ַ�������
                                                 
    3,                       //�豸���к��ַ�������
                                                
    0x01                  //����豸�ж��ٸ�����
  }
  ; 
//��������
const uint8_t Relay_ConfigDescriptor[RELAY_SIZ_CONFIG_DESC] =
  {
    0x09, 																												//������������С
    USB_CONFIGURATION_DESCRIPTOR_TYPE, //����������
    RELAY_SIZ_CONFIG_DESC,    //�ܳ���
    0x00,					//����ܳ���
    0x01,         //�ܽӿ���
    0x01,         //���õ�ֵ
    0x00,      	//���������õ��ַ���������������
    0xE0,         //����ģʽѡ��  (0xE0)
    0x32,        //�豸��Ҫ������100 mA
										//��λ2mA
    /**************�ӿ�����****************/
    /* 09 */
    0x09,      
    USB_INTERFACE_DESCRIPTOR_TYPE,//�ӿ�������
    0x00,         //����ǵ�0�Žӿ�
    0x00,         //���ýӿڱ��
    0x02,         //ʹ�õķ�0�˵���
		
		/***USB HID�Զ����豸***/
    0x03,         //���ʹ��� HID
    0x00,         //�����ʹ���
    0x00,         //Э�����
		
    0,            		//��Ҫ�ַ�������
		

		  /********************HID����********************/
    /* 18 */
    0x09,       
    HID_DESCRIPTOR_TYPE, //HID
    0x10,0x01,     //HID�汾�ţ�BCD��
    0x21,         //�豸���ڹ��ҵĹ��ұ�ţ���������
    0x01,         //HID������������Ŀ������1��
		
		//=====����������1=====//
    0x22,     //0x22������������0x23����������
    RELAY_SIZ_REPORT_DESC,0x00,//����������1��С
		//====================//
		
   
		
		 /******************** �˵�����********************/
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, //�˵�������
		
		//�豸�˵��ַ ��bit7:1-->IN  0-->OUT��bit[3:0] �˵�š�
    0x01,       //�˵�1��OUT   
	
    0x03,          //�˵����� ��0x00���ƣ�0x01ͬ�� 0x02���� 0x03�жϣ�
    0x10, 0x00,   //�ö˵���ջ��͵������Ϣ�� //��Ҫд0XFF
    				
    0x01,          //��ѯ���͵�ʱ��������λms����������ƿɺ��ԣ�ͬ������Ϊ1���жϿ���ѡ1~255��
    /* 34 */
		
	  /******************** �˵�����********************/
    
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, //�˵�������
		
		//�豸�˵��ַ ��bit7:1-->IN  0-->OUT��bit[3:0] �˵�š�
    0x82,       //�˵�2��IN
	
    0x03,          //�˵����� ��0x00���ƣ�0x01ͬ�� 0x02���� 0x03�жϣ�
    0x10, 0x00,   //�ö˵���ջ��͵������Ϣ�� //��Ҫд0XFF
    				
    0x01,          //��ѯ���͵�ʱ��������λms����������ƿɺ��ԣ�ͬ������Ϊ1���жϿ���ѡ1~255��
    /* 41 */
		
		
		
  }
	
  ;
	//����������
const uint8_t Relay_ReportDescriptor[RELAY_SIZ_REPORT_DESC] =
  {
    0x05,0x01,         /*Usage Page(Generic Desktop)*/
    
    0x09,0x00,        /*Usage(0)*/ //����0x00����ʶ��Ϊ�����豸
    
				0xA1,0x01,          /*Collection(Application)*/
		
				//================================//
				0x15,0x00,          /*Logical Minimum(0)*/
				
				0x25, 0xFF,          /*Logical Maximum(255)*/
			 
				0x19,0x01,          /*Usage Minimum(1)*/
				
				0x29,0x08,         /*Usage Maximum(8)*/
			
				0x95,0x08,          /*Report Count(8)*/

				0x75,0x08,          /*Report Size(8)*/
				
				0x81,0x02,					 /*Input(Data Var Abs)*/
			//================================//
			
				0x19,0x01,          /*Usage Minimum(1)*/
			
				0x29,0x08,         /*Usage Maximum(8)*/
				
				0x91,0x02,				 /*Output(Data Var Abs)*/
				//================================//
				
		0xC0									/*End Collection*/
  }
  ; 
//����ID����
const uint8_t Relay_StringLangID[RELAY_SIZ_STRING_LANGID] =
  {
    RELAY_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,//����Ӣ��ID
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */
//��Ӧ��
const uint8_t Relay_StringVendor[RELAY_SIZ_STRING_VENDOR] =
  {
		RELAY_SIZ_STRING_VENDOR,
		03,
		16,98, //��
		253,144,//��
		225,79,//��
		111,96,//Ϣ
		229,93,//��
		11,122,//��
		39,89,//��
		102,91,//ѧ
  };
//�豸����
const uint8_t Relay_StringProduct[RELAY_SIZ_STRING_PRODUCT] =
{
18,
	03,
	243,141,//��
	145,152,//Ƶ
	228,110,//��
	226,108,//��
	104,86,//��
	167,99,//��
	54,82,//��
	104,86//��
};
//��Ʒϵ������
uint8_t Relay_StringSerial[RELAY_SIZ_STRING_SERIAL] =
  {
		18,
		03,
		213,107,//��
		26,78,//ҵ
		190,139,//��
		161,139,//��
		86,0,//V
		49,0,//1
		46,0,//.
		48,0,//0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

