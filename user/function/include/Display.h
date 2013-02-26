/**
  ******************************************************************************
  * @file FSMC/SRAM/fsmc_sram.h 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Header for fsmc_sram.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
//#include "fsmc_sram.h"
//#include "flash.h"
//#include "lcd_dis24.h"


/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint16_t StartX;
	uint16_t StartY;
	uint16_t EndX;
	uint16_t EndY;
	uint8_t Direction;
	uint32_t Offset;
} LCD_CoordInfo;

typedef struct
{
	uint32_t realStartAddr;
	uint8_t	charCount;
	uint8_t bitMap[256];
}Bitmap;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LCD_ResInit(void);
//void LCD_DisplayPic(LCD_CoordInfo *, FlashDataInfoData_16 *);
//void LCD_DisplayFlashPic(LCD_CoordInfo *DisInfo, FlashDataInfoData_16 *FlashDisData);
void LCD_Battery(LCD_CoordInfo *DisInfo, unsigned char *gBattery);
void LCD_Icon(LCD_CoordInfo *DisInfo, uint8_t *gBattery);
void Time_Display(u32 TimeVar, LCD_CoordInfo *DisInfo);
void CleanLCD(LCD_CoordInfo *DisInfo, uint16_t bColor);
