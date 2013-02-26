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
#include "usart.h"
//#include "lcd_dis24.h"
#include "lcd_ILI9341.h"
#include "Display.h"
#include "iButton.h"

extern ComDataStruct ComData;
extern uint8_t ACK_Data[];
extern uint8_t DataSizeOfTransfer;
extern FlashDataInfoData_16 FLSH_PicData, FLSH_AllPicData, FLSH_NameData, FLSH_DepData, FLSH_KeyData, FLSH_ProData, FLSH_JobNumData;
extern LCD_CoordInfo DisInfoPic, DisInfoName, DisInfoDep, DisInfoProfession, DisInfoJobNum;
uint8_t KeyID[8];
uint8_t ReadKeyID[8];
extern FlashDataInfoData_16 FLSH_KeyData;

/* Private define ------------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
void SaveInfoToFlash(void);
bool PrasePakage(void);
bool CheckSum(void);
void CleanFlash(uint8_t Type);
void UpdateLCD(uint8_t Type);
void CleanDisplayInfo_on_LCD(uint8_t Type);
void GetKeyID_From_Flash(FlashDataInfoData_16 *FlashDisData);

/* Private variables ---------------------------------------------------------*/

void PersonalInfoMgt(void)
{
    if(ComData.RevOK == REV_OK)
    {
        switch(PrasePakage())
        {
        case ACK:
            SaveInfoToFlash();
			DataSizeOfTransfer = 5;
			ACK_Data[1] = ACK;
            ACK_Data[2] = ComData.Seq;
            ACK_Data[4] = ComData.Data[0];
            ACK_Data[5] = 0x00;
            ComData.LenNum = 0;
            ComData.DataCount = 0;
            ComData.Length = 0;
            /* Open USART1 to send acknowledged package */
            USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
            ComData.RevOK = REV_NORMAL;
            break;
        case NAK:
			DataSizeOfTransfer = 5;
			ACK_Data[1] = NAK;
            ACK_Data[2] = ComData.Seq;
            ACK_Data[4] = ComData.Data[0];
            ACK_Data[5] = 0x00;
            ComData.LenNum = 0;
            ComData.DataCount = 0;
            ComData.Length = 0;
            /* Open USART1 to send acknowledged package */
            USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
            ComData.RevOK = REV_NORMAL;
            break;
        case FINISH:
			DataSizeOfTransfer = 5;
			//do something
            CleanDisplayInfo_on_LCD(ComData.Data[0]);
            UpdateLCD(ComData.Data[0]);
            if(0x06 == ComData.Data[0])
            {
                GetKeyID_From_Flash(&FLSH_KeyData);
            }
            ACK_Data[1] = ACK;
            ACK_Data[2] = ComData.Seq;
            ACK_Data[4] = ComData.Data[0];
            ACK_Data[5] = 0x00;
            ComData.LenNum = 0;
            ComData.DataCount = 0;
            ComData.Length = 0;
            ComData.Seq = 0;
            /* Open USART1 to send acknowledged package */
            USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
            ComData.RevOK = REV_NORMAL;
            break;
        case CLEAN:
            DataSizeOfTransfer = 5;
			CleanFlash(ComData.Data[0]);
            ACK_Data[1] = ACK;
            ACK_Data[2] = ComData.Seq;
            ACK_Data[4] = ComData.Data[0];
            ACK_Data[5] = 0x00;
            ComData.LenNum = 0;
            ComData.DataCount = 0;
            ComData.Length = 0;
            /* Open USART1 to send acknowledged package */
            USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
            ComData.RevOK = REV_NORMAL;
			break;
		case UPLOAD_KEY:
			DataSizeOfTransfer = 13;
			ACK_Data[1] = ACK;
            ACK_Data[2] = ComData.Seq;
            ACK_Data[4] = ComData.Data[0];
            ACK_Data[5] = ReadKeyID[0];
			ACK_Data[6] = ReadKeyID[1];
			ACK_Data[7] = ReadKeyID[2];
			ACK_Data[8] = ReadKeyID[3];
			ACK_Data[9] = ReadKeyID[4];
			ACK_Data[10] = ReadKeyID[5];
			ACK_Data[11] = ReadKeyID[6];
			ACK_Data[12] = ReadKeyID[7];
            ComData.LenNum = 0;
            ComData.DataCount = 0;
            ComData.Length = 0;
            /* Open USART1 to send acknowledged package */
            USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
            ComData.RevOK = REV_NORMAL;
			break;
        }
    }
}

void GetKeyID_From_Flash(FlashDataInfoData_16 *FlashDisData)
{
    uint16_t *pBuffer = (uint16_t*)(FlashDisData->StartAddr);
    uint32_t Count = 0;
    uint16_t Sum = 4;
    /* Read the corectness of written data */
    for(Count = 0; Count < Sum; Count++)
    {
        KeyID[(Count * 2)] = (uint8_t)(*pBuffer >> 8);
        KeyID[(Count * 2) + 1] = (uint8_t)(*pBuffer & 0xff);
        pBuffer += 1;
    }
}

void CleanDisplayInfo_on_LCD(uint8_t Type)
{
    switch(Type)
    {
    case 0x01:
        CleanLCD(&DisInfoPic, 0xffff);
        break;
    case 0x02:
        CleanCharFromLCD(&DisInfoName, 0xffff);
        break;
    case 0x03:
        CleanCharFromLCD(&DisInfoDep, 0xffff);
        break;
    case 0x04:
        CleanCharFromLCD(&DisInfoProfession, 0xffff);
        break;
    case 0x05:
        CleanCharFromLCD(&DisInfoJobNum, 0xffff);
        break;
    }
}

void UpdateLCD(uint8_t Type)
{
    switch(Type)
    {
    case 0x01:
        LCD_DisplayFlashPic(&DisInfoPic, &FLSH_PicData);
        break;
    case 0x02:
        LCD_DisplayCharFromFlash(&DisInfoName, &FLSH_NameData);
        break;
    case 0x03:
        LCD_DisplayCharFromFlash(&DisInfoDep, &FLSH_DepData);
        break;
    case 0x04:
        LCD_DisplayCharFromFlash(&DisInfoProfession, &FLSH_ProData);
        break;
    case 0x05:
        LCD_DisplayCharFromFlash(&DisInfoJobNum, &FLSH_JobNumData);
        break;
    default:
        break;
    }
}

void CleanFlash(uint8_t Type)
{
    switch(Type)
    {
    case 0x01:
        FLSH_PicData.Offset = 0;
        EraseFlash_by_Page(&FLSH_AllPicData);
        break;
    case 0x02:
        FLSH_NameData.Offset = 0;
        EraseFlash_by_Page(&FLSH_NameData);
        break;
    case 0x03:
        FLSH_DepData.Offset = 0;
        EraseFlash_by_Page(&FLSH_DepData);
        break;
    case 0x04:
        FLSH_ProData.Offset = 0;
        EraseFlash_by_Page(&FLSH_ProData);
        break;
    case 0x05:
        FLSH_JobNumData.Offset = 0;
        EraseFlash_by_Page(&FLSH_JobNumData);
        break;
    case 0x06:
        FLSH_KeyData.Offset = 0;
        EraseFlash_by_Page(&FLSH_KeyData);
        break;
    }
}

uint8_t PrasePakage(void)
{
    if((ComData.Command <= 0) || (ComData.Command > 6))
    {
        return NAK;
    }
	if(ComData.Command == 0x06)
	{
		return UPLOAD_KEY;
	}
    if(ComData.Command == 0x02)
    {
        return FINISH;
    }
    if(ComData.Command == 0x05)
    {
        return CLEAN;
    }
    if(CheckSum() != ComData.CheckSum)
    {
        return NAK;
    }
    if((ComData.Seq != 0x01) && (ComData.Seq != (ComData.LastSeq + 1)))
    {
        return NAK;
    }
    return ACK;
}

uint8_t  CheckSum(void)
{
    char checkSum = 0;
    uint16_t i;

    for(i = 0; i < ComData.Length; i++)
    {
        checkSum += ComData.Data[i];
    }
    return checkSum;
}

void SaveData(FlashDataInfoData_16 *FlashOptionDataStruct)
{
    uint16_t  i;
    uint8_t *p;
    p = (uint8_t*)ComData.Data;
    p++;                        //jump the data cmd

    for(i = 0; i < (ComData.Length - 1) / 2; i++)
    {
        FlashOptionDataStruct->Data[i] = (D8toD16(&*p++, &*p++));
    }
    if(((ComData.Length - 1) % 2) == 1)
    {
        FlashOptionDataStruct->Data[i] = ((*p << 8) | 0x00ff);
        FlashOptionDataStruct->DataSize = ((ComData.Length - 1) / 2) + 1;
    }
    else
    {
        FlashOptionDataStruct->DataSize = ((ComData.Length - 1) / 2);
    }
    WriteFlash_by_16Bit(FlashOptionDataStruct);
}

void SaveInfoToFlash(void)
{

    switch(ComData.Data[0])
    {
    case 0x01:
        SaveData(&FLSH_PicData);
        break;
    case 0x02:
        SaveData(&FLSH_NameData);
        break;
    case 0x03:
        SaveData(&FLSH_DepData);
        break;
    case 0x04:
        SaveData(&FLSH_ProData);
        break;
    case 0x05:
        SaveData(&FLSH_JobNumData);
        break;
    case 0x06:
        SaveData(&FLSH_KeyData);
        break;
    }
}

