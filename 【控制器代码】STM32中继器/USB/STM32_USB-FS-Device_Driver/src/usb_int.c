/**
  ******************************************************************************
  * @file    usb_int.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    28-August-2012
  * @brief   Endpoint CTR (Low and High) interrupt's service routines
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t SaveRState;
__IO uint16_t SaveTState;

/* Extern variables ----------------------------------------------------------*/
extern void (*pEpInt_IN[7])(void);    /*  Handles IN  interrupts   */
extern void (*pEpInt_OUT[7])(void);   /*  Handles OUT interrupts   */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CTR_LP.
* Description    : Low priority Endpoint Correct Transfer interrupt's service
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_LP(void)
{
  __IO uint16_t wEPVal = 0;

  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)//��ȡ�жϼĴ������Ƿ���ȷ����
  {
    
    EPindex = (uint8_t)(wIstr & ISTR_EP_ID);//��ȡ�����жϵĶ˵����
    if (EPindex == 0)//�˵�0
    {

      
	    SaveRState = _GetENDPOINT(ENDP0);//��ȡ�˵�0״̬�Ĵ���
			
	    SaveTState = SaveRState & EPTX_STAT;//����˵�0����״̬
	    SaveRState &=  EPRX_STAT;	//����˵�0����״̬

	    _SetEPRxTxStatus(ENDP0,EP_RX_NAK,EP_TX_NAK);//����RX��TX����NAK, ���ݴ���������

    
			//��PC��������
      if ((wIstr & ISTR_DIR) == 0)//IN����
      {
        _ClearEP_CTR_TX(ENDP0);//����˵�0��ȷ���ͱ�־λ
        In0_Process();//����IN���ư�
			
				_SetEPRxTxStatus(ENDP0,SaveRState,SaveTState);//���ö˵�0���ͽ���״̬
		  return;
      }
      else//����PC����
      {
        wEPVal = _GetENDPOINT(ENDP0);
        //=======Setup��==========//
        if ((wEPVal &EP_SETUP) != 0)
        {
          _ClearEP_CTR_RX(ENDP0);//����˵�0���ձ�־
          Setup0_Process();//���ݴ���
					
		      _SetEPRxTxStatus(ENDP0,SaveRState,SaveTState);//���ö˵�0���ͽ���״̬
          return;
        }
				//=======OUT��==========//
        else if ((wEPVal & EP_CTR_RX) != 0)
        {
          _ClearEP_CTR_RX(ENDP0);//����˵�0���ձ�־
          Out0_Process();//OUT�����ݴ���
          /* before terminate set Tx & Rx status */
     
		     _SetEPRxTxStatus(ENDP0,SaveRState,SaveTState);//���ö˵�0���ͽ���״̬
          return;
        }
      }
    }
    else//��0�˵�
    {
    
      wEPVal = _GetENDPOINT(EPindex);
      if ((wEPVal & EP_CTR_RX) != 0)//��ȷ����
      {
        
        _ClearEP_CTR_RX(EPindex);//������ձ�־

        //=====================================//
        (*pEpInt_OUT[EPindex-1])();//OUT�ص���������

      } 

      if ((wEPVal & EP_CTR_TX) != 0)//��ȷ����
      {
       
        _ClearEP_CTR_TX(EPindex);//������ͱ�־

         //=====================================//
        (*pEpInt_IN[EPindex-1])();//IN�ص���������
      } /* if((wEPVal & EP_CTR_TX) != 0) */

    }/* if(EPindex == 0) else */

  }/* while(...) */
}

/*******************************************************************************
* Function Name  : CTR_HP.
* Description    : High Priority Endpoint Correct Transfer interrupt's service 
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_HP(void)
{
  uint32_t wEPVal = 0;

  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)
  {
    _SetISTR((uint16_t)CLR_CTR); /* clear CTR flag */
    /* extract highest priority endpoint number */
    EPindex = (uint8_t)(wIstr & ISTR_EP_ID);
    /* process related endpoint register */
    wEPVal = _GetENDPOINT(EPindex);
    if ((wEPVal & EP_CTR_RX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_RX(EPindex);

      /* call OUT service function */
      (*pEpInt_OUT[EPindex-1])();

    } /* if((wEPVal & EP_CTR_RX) */
    else if ((wEPVal & EP_CTR_TX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_TX(EPindex);

      /* call IN service function */
      (*pEpInt_IN[EPindex-1])();


    } /* if((wEPVal & EP_CTR_TX) != 0) */

  }/* while(...) */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
