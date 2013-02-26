/**
  ******************************************************************************
  * @file RTC/Calendar/main.c
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main program body
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
#include "include.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TxBufferSize   (sizeof(TxBuffer))                  //统计发送数组元素个数
#define RxBufferSize   (sizeof(TxBuffer))                  //接收数组元素个数与发送数组一致

/* Lock State */
#define OPEN 0x01
#define CLOSE 0x02

#ifdef _GNUC_
#define PUTCHAR_PROTOTYPE int _io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
extern uint8_t KeyID[8];
extern uint8_t ReadKeyID[8];
uint8_t TxBuffer[8];     //发送数组
uint8_t RxBuffer[RxBufferSize];                      //接收数组
uint8_t DataSizeOfTransfer = 5;           //发送数据量等于发送数组元素个数
uint8_t DataSizeOfReceiver = RxBufferSize;           //接收数据量等于接收数组元素个数
uint8_t ACK_Data[13] = {0x7e, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
ComDataStruct ComData;
extern unsigned char gBattery[6][4608], gLock[3][4608];
static uint8_t LockState = CLOSE;

LCD_CoordInfo DisInfoPic = {10, 10, 137, 180, 'U', 0},

              DisInfoName = {147, 39, 319, 63, 'U', 0},
              DisInfoDep = {147, 73, 319, 97, 'U', 0},
			  DisInfoProfession = {147, 107, 319, 131, 'U', 0},
			  DisInfoJobNum = {147, 141, 319, 165, 'U', 0},
			  
			  DisInfoState = {147, 175, 219, 199, 'U', 0},
			  DisInfoStateNow = {220, 175, 319, 199, 'U', 0},
			  
			  DisInfoTime = {160, 223, 319, 247, 'u', 0},
			  
              DisInfoBattery = {192, 20, 239, 67, 'R', 0},
              DisInfoLock = {90, 187, 137, 234, 'u', 0};

LCD_CoordInfo DisInfoIButtonKey = {192, 20, 239, 67, 'R', 0};                            

FlashDataInfoData_16 FLSH_PicData = {0x08040000, 0x08065fff, 77823, 0},
                     FLSH_AllPicData = {0x08040000, 0x08065fff, 77823, 0},
					 
                     FLSH_NameData = {0x08066000, 0x0806afff, 10240, 0},
                     FLSH_ALLNameData = {0x08066000, 0x0806afff, 10240, 0},
					 
                     FLSH_DepData = {0x0806b000, 0x0806ffff, 10240, 0},
                     FLSH_AllDepData = {0x0806b000, 0x0806ffff, 10240, 0},										
					 
					 FLSH_ProData = {0x08070000, 0x08074fff, 10240, 0},
                     FLSH_AllProData = {0x08070000, 0x08074fff, 10240, 0},	

					 FLSH_JobNumData = {0x08075000, 0x08079fff, 10240, 0},
                     FLSH_AllJobNumData = {0x08075000, 0x08079fff, 10240, 0},					 
					 
                     FLSH_KeyData = {0x0807a000, 0x0807efff, 10240, 0},
                     FLSH_AllKeyData = {0x0807a000, 0x0807efff, 10240, 0};

TMR       TmrTbl[TMR_MAX_TMR];          /* Table of timers managed by this module             */

/* Private function prototypes -----------------------------------------------*/
void uDelay(uint16_t nTime);
void delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/*
*********************************************************************************************************
*                               Function executed when Timers Time Out
*********************************************************************************************************
*/
static  void  BatteryFaultTimer (void *arg)
{
    arg = arg;
	if(DisInfoBattery.Offset >= 1)
    {        
		DisInfoBattery.Offset = 0;
		
		CleanLCD(&DisInfoStateNow, 0xffff);
		LCD_Icon(&DisInfoBattery, gBattery[0]);
    } else if(DisInfoBattery.Offset == 0)
	{
		LCD_DisplayChinese(&DisInfoStateNow, "矿灯故障", 0xf800, 0xffff);
		LCD_Icon(&DisInfoBattery, gBattery[6]);
		DisInfoBattery.Offset++;
	}    
}

static  void  BatteryDisplayTimer (void *arg)
{
    arg = arg;
    if(DisInfoBattery.Offset == 5)
    {
        DisInfoBattery.Offset = 0;
    }
    LCD_Icon(&DisInfoBattery, gBattery[DisInfoBattery.Offset]);
    DisInfoBattery.Offset++;
}

static  void  ConmmunctionTimer (void *arg)
{
    arg = arg;
    ComData.CheckSum = 0;
    ComData.ComState = START;
    ComData.LenNum = 0;
    ComData.DataCount = 0;
    ComData.Length = 0;
    ComData.LastSeq = 0;
    ComData.Seq = 0;
    ComData.RevOK = REV_NORMAL;
}

static void BackLightTimeOutTimer(void *arg)
{
	arg = arg;
	CloseBackLight();
}

static void LockOffDelayTimer(void *arg)
{
	CleanLCD(&DisInfoLock, 0xffff);//清除锁图标
	GPIO_WriteBit(GPIOE, GPIO_Pin_6, (BitAction)0x01);	//H //Low
}


TmrTaskConfig(void)
{
    TmrInit();
    TmrCfgFnct(FALSE, 0, ConmmunctionTimer, (void *)0);                  /* Execute when Timer #0 times out          */
    TmrCfgFnct(TRUE, 1, BatteryDisplayTimer, (void *)0);                  /* Execute when Timer #0 times out          */
	TmrCfgFnct(TRUE, 2, BatteryFaultTimer, (void *)0);                  /* Execute when Timer #0 times out          */
	TmrCfgFnct(FALSE, 4, BackLightTimeOutTimer, (void *)0);                  /* Execute when Timer #0 times out          */
	//added by zhuxs 2012-8-7
	TmrCfgFnct(FALSE, 3, LockOffDelayTimer, (void *)0);                  /* Execute when Timer #0 times out          */	

	TmrSetMST(1, 0, 0, 5);                                 /* Set timer #0 to 0 min., 0 sec. 5/10 sec. */
    TmrSetMST(0, 0, 10, 0);                                 /* Set timer #0 to 0 min., 10 sec. 0/10 sec. */
	TmrSetMST(2, 0, 0, 5);
	TmrSetMST(4, 0, 30, 0);
	//added by zhuxs 2012-8-7	
	TmrSetMST(3, 0, 2, 0);	
    TmrStart(1);
}

/*******************************************************************************
* Function Name  : Time_Adjust
* Description    : Adjusts time.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Time_Reset(void)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Change the current time */
    RTC_SetCounter(0);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}

void RTC_Init(void)
{
    /* RTC Configuration */
    RTC_Configuration();
    /* Adjust time by values entred by the user on the hyperterminal */
    Time_Reset();
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Clear reset flags */
    RCC_ClearFlag();
}

char toHexChar(int num)
{
	unsigned char output;
	switch(num)
	{
		case 10:
			output = 'A';
			break;
		case 11:
			output = 'B';
			break;
		case 12:
			output = 'C';
			break;
		case 13:
			output = 'D';
			break;
		case 14:
			output = 'E';
			break;
		case 15:
			output = 'F';
			break;
		default:
			output = num;	
	}

	return output;
}

void iButtonKeyID_Save()
{
	uint8_t i = 0;

	if(OWTouchReset())
    {
        OWWriteByte(0x33);
        for(i = 0 ; i < 8; i++)
        {
            ReadKeyID [i] = OWReadByte();
            uDelay(20);           
        }        
    }
}

bool iButtonCheck(void)
{   
    uint8_t i = 0;
	uint8_t tmpKey[8];

    if(OWTouchReset())
    {
        OWWriteByte(0x33);
        for(i = 0 ; i < 8; i++)
        {
            tmpKey [i] = OWReadByte();
            uDelay(20);
            if(tmpKey [i] != KeyID[i])
            {
                return FALSE;
            }
            //printf("%d", SerialNum[i])
        }        
		return TRUE;
    }

    return FALSE;
}

void LED_Test(void)
{
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, (BitAction)0x01);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)0x01);
	GPIO_WriteBit(GPIOE, GPIO_Pin_0, (BitAction)0x01);
	delay(2000);
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, (BitAction)0x00);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)0x00);
	GPIO_WriteBit(GPIOE, GPIO_Pin_0, (BitAction)0x00);
}

void OpenLock(void)
{
	GPIO_WriteBit(GPIOE, GPIO_Pin_6, (BitAction)0x00);	//L // High
	//delay(500);	
}

void InitLock(void)
{
	GPIO_WriteBit(GPIOE, GPIO_Pin_6, (BitAction)0x01);	//H
}

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */
int main(void)
{
    SystemResourcesInit(RCC_INIT
                        | NVIC_INIT
                        | FSCM_INIT
                        | FLASH_INIT
                        | USART_INIT
                        | GPIO_INIT
                        | SYSTICK_INIT
                        | TIMER_INIT);
	InitLock();
	
	InitBatteryStatus();

//	LED_Test();
    TmrTaskConfig();
	
    LCD_ResInit();
	
    GetKeyID_From_Flash(&FLSH_KeyData);		
	
    /* Background */
    //LCD_BackgroundUpdata(0xff00);     //Yellow
    //LCD_BackgroundUpdata(0xf800); 		//Red
	//LCD_BackgroundUpdata(0x001f); 		//Blue
	LCD_BackgroundUpdata(0xffff);     //White
    /* Pic */
    LCD_DisplayFlashPic(&DisInfoPic, &FLSH_PicData);
    /* Name and Info */
    LCD_DisplayCharFromFlash(&DisInfoName, &FLSH_NameData);
    LCD_DisplayCharFromFlash(&DisInfoDep, &FLSH_DepData);
	LCD_DisplayCharFromFlash(&DisInfoProfession, &FLSH_ProData);
	LCD_DisplayCharFromFlash(&DisInfoProfession, &FLSH_ProData);
	LCD_DisplayCharFromFlash(&DisInfoJobNum, &FLSH_JobNumData);
    /* State */
    LCD_DisplayChinese(&DisInfoState, "状态：", 0x0000, 0xffff);
	/* Lock */
	//deleted by zhuxs 2012-8-7
	//LCD_Icon(&DisInfoLock, gLock[0]);
    /* RTC */
    RTC_Init();
    //printf("\n\r重庆DQ电子串口中断方式发送和接收测试程序\n\r");
    //printf("\n\r请发送一个大于等于8位的字符串\n\r");

	while(1)
    {
        PersonalInfoMgt();
		iButtonKeyID_Save();
        if((iButtonCheck() == TRUE) && (LockState == CLOSE))
        {			
			TmrStart(4);
			OpenBackLight();
			
			LCD_Icon(&DisInfoLock, gLock[1]);	//锁开		
			LockState = OPEN;
			OpenLock();
			TmrStart(3);//启动开锁超时图标显示定时器
        } 
		else if((iButtonCheck() == FALSE) && (LockState == OPEN))
        {
        	LockState = CLOSE;
        } 
		TmrTask(0);
        Time_Display(RTC_GetCounter(), &DisInfoTime);
        BatteryManage();
    } 
}

/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : 重定义C语言的fprint函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE

{
    /* Write a character to the USART */
    USART_SendData(USART1, (uint8_t)ch);

    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;

    return ch;
}

/**
 * @brief  Inserts a delay time.
 * @param nTime: specifies the delay time length, in milliseconds.
 * @retval : None
 */
void delay(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/**
 * @brief  Inserts a delay time.
 * @param nTime: specifies the delay time length, in milliseconds.
 * @retval : None
 */
void uDelay(uint16_t nTime)
{
    uint16_t TIMCounter = nTime;

    TIM_Cmd(TIM2, ENABLE);
    TIM_SetCounter(TIM2, TIMCounter);
    while (TIMCounter > 1)
    {
        TIMCounter = TIM_GetCounter(TIM2);
    }
    TIM_Cmd(TIM2, DISABLE);
}

/**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval : None
*/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    while (1)
    {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
