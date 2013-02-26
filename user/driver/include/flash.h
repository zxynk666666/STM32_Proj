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
typedef struct
{
  __IO uint32_t StartAddr;
  __IO uint32_t EndAddr;
  __IO uint32_t DataSize;
  uint32_t Data[2048];
} FlashDataInfoData_32;

typedef struct
{
  __IO uint32_t StartAddr;
  __IO uint32_t EndAddr;
  __IO uint32_t DataSize;
  __IO uint32_t Offset;
  uint16_t Data[2048];
} FlashDataInfoData_16;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void EraseFlash_by_Page(FlashDataInfoData_16 *FlashOptionDataStruct);
void ReadFlash_by_32Bit(FlashDataInfoData_32 *);
void WriteFlash_by_32Bit(FlashDataInfoData_32 *);
void ReadFlash_by_16Bit(FlashDataInfoData_16 *);
void WriteFlash_by_16Bit(FlashDataInfoData_16 *);

