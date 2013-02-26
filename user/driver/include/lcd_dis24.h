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

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void LCD_Init(void);										//≥ı ºªØLCD
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);

void LCD_WR_Data(unsigned int val);
void LCD_Test(unsigned int a);
void ini(void);

void LCD_WR_ZF(unsigned int a, unsigned int b, unsigned int a1,unsigned int b1, unsigned int d,unsigned int e, unsigned char g, unsigned char *f); 
void LCD_WR_Pixel(unsigned int a, unsigned int b, unsigned int e) ;
unsigned char *num_pub(unsigned int a);
unsigned int D8toD16(unsigned char *Dl,unsigned char *Dh) ;
unsigned long mypow(unsigned char m,unsigned char n);
void TFT_ShowChar(unsigned char postion_x,unsigned int postion_y,unsigned char num,unsigned int fColor, unsigned int bColor);
void LCD_Test(unsigned int a);
void LCD_Control(uint16_t StartX, uint16_t EndX, uint16_t StartY, uint16_t EndY, unsigned char direction);
