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
#include "flash.h"
#include "platform_config.h"

/* Private define ------------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
void EraseFlash_by_Page(FlashDataInfoData_16 *FlashOptionDataStruct)
{
	__IO uint32_t StartAddr = FlashOptionDataStruct->StartAddr;
	__IO uint32_t EndAddr = FlashOptionDataStruct->StartAddr + FlashOptionDataStruct->DataSize * 2;
	uint32_t EraseCounter;
	__IO uint32_t NbrOfPage;
	volatile FLASH_Status FLASHStatus;
	FLASHStatus = FLASH_COMPLETE;
	/* Unlock the Flash Program Erase controller */
	FLASH_Unlock();
	/* Define the number of page to be erased */
	NbrOfPage = (EndAddr - StartAddr) / FLASH_PAGE_SIZE;
	NbrOfPage = (NbrOfPage > 0) ? NbrOfPage : 1;
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	/* Erase the FLASH pages */
	for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
	{
		FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * EraseCounter));
	}
}

void WriteFlash_by_32Bit(FlashDataInfoData_32 *FlashOptionDataStruct)
{
	uint32_t Address = 0x00;
	volatile FLASH_Status FLASHStatus;
	uint32_t Count = 0;
	FLASHStatus = FLASH_COMPLETE;
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	/*  FLASH Word program of data 0x15041979 at addresses defined by StartAddr and EndAddr*/
	Address = FlashOptionDataStruct->StartAddr;
	for(Count = 0; (Count < FlashOptionDataStruct->DataSize) && (FLASHStatus == FLASH_COMPLETE); Count++)
	{
		FLASHStatus = FLASH_ProgramWord(Address, FlashOptionDataStruct->Data[Count]);
		Address += 4;
	}
}

void ReadFlash_by_32Bit(FlashDataInfoData_32 *FlashOptionDataStruct)
{
	uint32_t Address = 0x00;
	uint32_t Count = 0;
	/* Read the corectness of written data */
	Address = FlashOptionDataStruct->StartAddr;
	for(Count = 0; (Count < FlashOptionDataStruct->DataSize); Count++)
	{
		FlashOptionDataStruct->Data[Count] = (*(__IO uint16_t*) Address);
		Address += 4;
	}
}

void ReadFlash_by_16Bit(FlashDataInfoData_16 *FlashOptionDataStruct)
{
	uint32_t Address = 0x00;
	uint32_t Count = 0;
	/* Read the corectness of written data */
	Address = FlashOptionDataStruct->StartAddr;
	for(Count = 0; (Count < FlashOptionDataStruct->DataSize); Count++)
	{
		FlashOptionDataStruct->Data[Count] = (*(__IO uint16_t*) Address);
		Address += 2;
	}
}

void WriteFlash_by_16Bit(FlashDataInfoData_16 *FlashOptionDataStruct)
{
	uint32_t Address = 0x00;
	volatile FLASH_Status FLASHStatus;
	uint32_t Count = 0;
	FLASHStatus = FLASH_COMPLETE;
	/*  FLASH Word program of data at addresses defined by StartAddr and EndAddr*/
	
	for(Count = 0; (Count < FlashOptionDataStruct->DataSize) && (FLASHStatus == FLASH_COMPLETE); Count++)
	{
		Address = FlashOptionDataStruct->StartAddr + FlashOptionDataStruct->Offset;
		FLASHStatus = FLASH_ProgramHalfWord(Address, FlashOptionDataStruct->Data[Count]);
		FlashOptionDataStruct->Offset += 2;
	}

}
