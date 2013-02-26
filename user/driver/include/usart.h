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


#define SOF     	0x7e
#define START   	0
#define CMD     	1
#define SEQ     	2
#define LEN     	3
#define DATA        4
#define CKS        5

#define REV_NORMAL		0x00
#define REV_OK			0x01
#define REV_FAULT       0x02

#define	DWLOAD		0x01
#define	FINISH		0x02
#define	ACK			0x03
#define	NAK			0x04
#define	CLEAN		0x05
#define	UPLOAD_KEY	0x06



/* Exported types ------------------------------------------------------------*/
typedef struct
{
	__IO uint8_t ComState;
	__IO uint8_t Command;
	__IO uint8_t LastSeq;
	__IO uint8_t Seq;
	__IO uint8_t LenNum;
	__IO uint16_t Length;
	__IO uint16_t DataCount;
	uint8_t Data[4096];
	__IO uint8_t CheckSum;
	__IO uint8_t RevOK;
} ComDataStruct;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


