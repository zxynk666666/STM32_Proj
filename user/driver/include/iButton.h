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


#define STANDARD
//#define SPEED 1
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

// 'tick' values

//-----------------------------------------------------------------------------
// Set the 1-Wire timing to 'standard' (standard=1) or 'overdrive' (standard=0).
//
#ifdef STANDARD
	// Standard Speed
	#define		A   (6)
	#define		B   (64)
	#define		C   (60)
	#define		D   (10)
	#define		E   (9)
	#define		F   (55)
	#define		G   0
	#define		H   (480)
	#define		I   (70)
	#define		J   (410)
#elif SPEED
	// Overdrive Speed
	#define		A 	(1.5)
	#define		B	(7.5)
	#define		C 	(7.5)
	#define		D 	(2.5)
	#define		E 	(0.75)
	#define		F 	(7)
	#define		G	(2.5)
	#define		H 	(70)
	#define		I	(8.5)
	#define		J 	(40)
#endif
/* Exported functions ------------------------------------------------------- */
void GPIO_Conf_in(void);
void GPIO_Conf_out(void);
uint8_t OWTouchReset(void);
void OWWriteBit(uint8_t bit);
uint8_t OWReadBit(void);
void OWWriteByte(uint8_t data);
uint8_t OWReadByte(void);
uint8_t OWTouchByte(uint8_t data);
void OWBlock(unsigned char *data, uint8_t data_len);
uint8_t OWOverdriveSkip(unsigned char *data, uint8_t data_len);

