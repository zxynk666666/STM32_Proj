/**
  ******************************************************************************
  * @file FSMC/SRAM/fsmc_sram.c
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  This file provides a set of functions needed to drive the
  *         IS61WV51216BLL SRAM memory mounted on STM3210E-EVAL board.
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
#include "Battery.h"
#include "Display.h"

/* Private define ------------------------------------------------------------*/
uint8_t InAlarm = 0x01;

/* Private typedef -----------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
Battery bStatus = {0, 0, 0 , 0 , 0 , 0};
extern unsigned char gBattery[6][4608];
extern LCD_CoordInfo DisInfoBattery, DisInfoStateNow;
void Alarm(void);
void ChargingStatus_LED_Control(uint8_t ChargingStatus);

void InitBatteryStatus(void)
{
	bStatus.BatteryStatus = NORMAL;
	bStatus.Flag = 0;
}

/* Private functions ---------------------------------------------------------*/
void Blue_LED_Toggle(InAlarm)
{
	if(InAlarm) 
	{
		GPIO_WriteBit(GPIOE, GPIO_Pin_0, (BitAction)0x01);	//turn on blue LED		
	} else {
		GPIO_WriteBit(GPIOE, GPIO_Pin_0, (BitAction)0x00);	//turn off blue LED	
	}
	
	InAlarm = ~InAlarm;
}
void ChargingStatus_LED_Control(uint8_t ChargingStatus)
{
    switch(ChargingStatus)
    {
    case CHARGING:
        GPIO_WriteBit(GPIOE, GPIO_Pin_0, (BitAction)0x00);	//turn off blue LED
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)0x00);	//turn off green LED
        GPIO_WriteBit(GPIOD, GPIO_Pin_6, (BitAction)0x01);	//turn on red LED
        break;
    case FULL:
        GPIO_WriteBit(GPIOE, GPIO_Pin_0, (BitAction)0x00);	//turn off blue LED
        GPIO_WriteBit(GPIOD, GPIO_Pin_6, (BitAction)0x00);	//turn off red LED
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)0x01);	//turn on green LED
        break;
    case REMOVE:
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)0x00);	//turn off green LED
        GPIO_WriteBit(GPIOD, GPIO_Pin_6, (BitAction)0x00);	//turn off red LED
        GPIO_WriteBit(GPIOE, GPIO_Pin_0, (BitAction)0x01);	//turn on blue LED
        break;
    default:
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)0x00);	//turn off green LED
        GPIO_WriteBit(GPIOD, GPIO_Pin_6, (BitAction)0x00);	//turn off red LED
        GPIO_WriteBit(GPIOE, GPIO_Pin_0, (BitAction)0x00);	//turn off blue LED
        break;
    }
}

void BatteryStatusDisplay(uint8_t status)
{
	CleanLCD(&DisInfoStateNow, 0xffff);
	switch(status)
	{
		case CHARGING:
			TmrStop(2);			
			TmrStart(1);
			LCD_DisplayChinese(&DisInfoStateNow, "正在充电", 0x0000, 0xffff);
			break;
		case FULL:
			TmrStop(2);
			TmrStop(1);
			LCD_Icon(&DisInfoBattery, gBattery[5]);
			LCD_DisplayChinese(&DisInfoStateNow, "已经充满", 0x0000, 0xffff);
			break;
		case REMOVE:
			TmrStop(1);
			TmrStop(2);
			LCD_Icon(&DisInfoBattery, gBattery[0]);
			LCD_DisplayChinese(&DisInfoStateNow, "矿灯取走", 0x0000, 0xffff);
			break;
		case BAD:
			TmrStop(1);
			TmrStart(2);
			break;
	}
}

void Battery_InCharging(void)
{
	BatteryStatusDisplay(CHARGING);

    ChargingStatus_LED_Control(bStatus.ChargingStatus);	
}

void Battery_Full(void)
{
	if(bStatus.THH < 1)				//如果充电时间小于1hour统计故障一次
	{
	    bStatus.ChargingCounter++;
	}
	else
	{
	    bStatus.ChargingCounter = 0;
	}
	BatteryStatusDisplay(FULL);

	ChargingStatus_LED_Control(bStatus.ChargingStatus);
}

void Battery_Removed(void)
{
	bStatus.ChargingCounter = 0;
	bStatus.BatteryStatus = NORMAL;
	bStatus.Flag = 0;

	BatteryStatusDisplay(REMOVE);

	ChargingStatus_LED_Control(bStatus.ChargingStatus);
}

void BatteryRemoveCheck(void)
{
	/* 电池移除 */
	if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) && !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5))           //检测PA0端口对应的按钮是否按下
    {
        if(bStatus.ChargingStatus != REMOVE)
        {						
			bStatus.ChargingStatus = REMOVE;

            Battery_Removed();           

            Time_Reset();
        }
    }
    
}

void StateCheck(void)
{
	
	/* 充电中 */
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) && GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5))           //检测PA0端口对应的按钮是否按下
    {
        if(bStatus.ChargingStatus != CHARGING)
        {
            bStatus.ChargingStatus = CHARGING;

            Battery_InCharging();

            Time_Reset();
        }

    }
    /* 电池充满 */
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) && !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))           //检测PA0端口对应的按钮是否按下
    {
        if(bStatus.ChargingStatus != FULL)
        {
            bStatus.ChargingStatus = FULL;

            Battery_Full();

            Time_Reset();		 //每次状态改变时timer复位一次
        }
    }
    
}

void GetBatteryStatusTime(void)
{
    uint32_t TimeVar = RTC_GetCounter();

    /* Compute  hours */
    bStatus.THH = TimeVar / 3600;
    /* Compute minutes */
    bStatus.TMM = (TimeVar % 3600) / 60;
    /* Compute seconds */
    bStatus.TSS = (TimeVar % 3600) % 60;

}

/* 电池故障检测函数
	判据:
	1. 如果电池连续充电5小时以上没有充满，则认为电池短路
	2. 如果连续3次不到一小时就充满，则认为电池损坏
*/
void FaultDetection(void)
{
    if((bStatus.ChargingStatus == CHARGING) && (bStatus.THH >= 5))
    {
		bStatus.BatteryStatus = BAD;
    }
    else if(bStatus.ChargingCounter > 3)
    {
		bStatus.BatteryStatus = BAD;
    }
}

void BatteryFaultAlarm(uint8_t toggle)
{
		TmrStop(1);
		TmrStart(2);
}

void BatteryManage(void)
{
	BatteryRemoveCheck();
	if((bStatus.BatteryStatus == BAD) && (bStatus.Flag == 0))
	{
		bStatus.Flag = 1;
		BatteryFaultAlarm(1);
	} else if(bStatus.BatteryStatus == NORMAL)
	{
		StateCheck();
	}
    GetBatteryStatusTime();	 //获取电池状态时间戳
    FaultDetection();	  //电池故障检测
}