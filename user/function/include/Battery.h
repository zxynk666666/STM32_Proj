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

/* Battery State */
#define CHARGING 	0x01
#define FULL 		0x02
#define REMOVE 		0x03

#define SHORT	0x04
#define BAD		0x05
#define NORMAL	0x00

/* Alarm State */
#define ON 		0x01
#define OFF		0x00

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint8_t ChargingStatus;
	uint8_t ChargingCounter;
	uint8_t BatteryStatus;
	uint8_t Flag;
	uint32_t	THH;
	uint32_t	TMM;
	uint32_t	TSS;
} Battery;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void StateCheck(void);