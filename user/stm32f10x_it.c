/**
  ******************************************************************************
  * @file RTC/Calendar/stm32f10x_it.c
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "platform_config.h"
#include "Usart.h"
#include "TimerTask.h"

/** @addtogroup StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_Calendar
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t TimeDisplay;

extern unsigned char  TxBuffer[];             //�������鶨���ȫ�ֱ���
extern unsigned char  RxBuffer[];             //�������鶨���ȫ�ֱ���
extern unsigned char  DataSizeOfTransfer;     //���������������ȫ�ֱ���
extern unsigned char  DataSizeOfReceiver;     //���������������ȫ�ֱ���
unsigned char  ReceiveComplete = 0;           //������ɱ�־λ
unsigned char  TxCounter = 0;                 //���ͼ�����
unsigned char  RxCounter = 0;                 //���ռ�����

extern ComDataStruct ComData;
extern uint8_t ACK_Data[];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
    TmrDiDa();
	TimingDelay_Decrement();
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval : None
  */
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        RTC_ClearITPendingBit(RTC_IT_SEC);
        /* Toggle GPIO_LED pin 6 each 1s */
        GPIO_WriteBit(GPIO_LED, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_LED, GPIO_Pin_6)));
        /* Enable time update */
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
        /* Reset RTC Counter when Time is 23:59:59 */
        if (RTC_GetCounter() == 0x00015180)
        {
            RTC_SetCounter(0x0);
            /* Wait until last write operation on RTC registers has finished */
            RTC_WaitForLastTask();
        }
    }
}

/******************************************************************************
                            �ӿ��豸�жϴ�����
  ��Ҫ����жϴ�����ʱ����Ӧ�ĺ�����λ�������ļ�startup_stm32f10x_hd.s
                       ���ж�������Vector Table
******************************************************************************/
void USART1_IRQHandler(void)                                        //����1�жϴ�����
{
    uint8_t Data;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)               //����Ƿ��ǽ����ж�
    {
        /* Read one byte from the receive data register */
        Data = USART_ReceiveData(USART1);               //����һ�ֽ�����
        switch(ComData.ComState)
        {
        case START:
            if(Data == 0x7e)
            {
                ComData.ComState = CMD;
               TmrReset(0);
               TmrStart(0);
            }
            break;
        case CMD:
            ComData.Command = Data;
            ComData.ComState = SEQ;
            break;
        case SEQ:
            if(0x01 == Data)
            {
                ComData.LastSeq = 1;
            }
            else
            {
                ComData.LastSeq = ComData.Seq;
            }
            ComData.Seq = Data;
            ComData.ComState = LEN;
            break;
        case LEN:
            if(ComData.LenNum == 1)
            {
                ComData.Length = (ComData.Length << 8) + Data;
                if(ComData.Length > 4096)
                {
                    ComData.Length = 1;
                }
                ComData.ComState = DATA;
                ComData.LenNum = 0;
            }
            else
            {
                ComData.Length = (ComData.Length << 8) + Data;
                ComData.LenNum++;
            }
            break;
        case DATA:
            ComData.DataCount++;
            if(ComData.DataCount == ComData.Length)
            {
                ComData.Data[ComData.DataCount - 1] = Data;
                ComData.DataCount = 0;
                ComData.ComState = CKS;
            }
            else
            {
                ComData.Data[ComData.DataCount - 1] = Data;
            }
            break;
        case CKS:
            ComData.CheckSum = Data;
            ComData.ComState = START;
            ComData.RevOK = REV_OK;
            TmrStop(0);
            break;
        }
    }
    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)             //����Ƿ��Ƿ����ж�
    {
        USART_ClearFlag(USART1, USART_FLAG_TC);
        /* Send one byte */
        USART_SendData(USART1, ACK_Data[TxCounter++]);                 //����һ�ֽ�����
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        if(TxCounter == DataSizeOfTransfer)                            //�������������������������
        {
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);               //�رմ��ڷ����жϣ�ֹͣ����
            //GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)(!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1)));
            TxCounter = 0;                                               //���ͼ���������
        }
    }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
