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
#include "display.h"
#include "flash.h"
#include "Fsmc_sram.h"
#include "lcd_ILI9341.h"
#include "GB2424.h"

/* Private define ------------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
extern void LCD_Init(void);
extern void LCD_Control(uint16_t StartX, uint16_t EndX, uint16_t StartY, uint16_t EndY, unsigned char direction);

extern unsigned char nAsciiDot[];

/* Private variables ---------------------------------------------------------*/

//LCD 背景设置
void LCD_BackgroundUpdata(unsigned char a)
{
	unsigned long n;

		n=0;
		//初始化屏幕起始坐标
		LCD_Control(0, 319, 0, 239, 'U');
		while(n < 76800)
		{
			LCD_WR_Data(a);
			n++;
	 	}
}

void OpenBackLight(void) 
{
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
}

void CloseBackLight(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
}

void LCD_ResInit(void)
{
	//GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	GPIO_SetBits(GPIOC, GPIO_Pin_7 );
    /* Configure FSMC Bank1 NOR/PSRAM */
    FSMC_LCD_Init();
    LCD_Init();
}

void LCD_DisplayFlashPic(LCD_CoordInfo *DisInfo, FlashDataInfoData_16 *FlashDisData)
{
    uint16_t *pBuffer = (uint16_t*)(FlashDisData->StartAddr);
    uint32_t Count = 0;
    uint16_t Sum;
    /* Read the corectness of written data */
    Sum = (DisInfo->EndX - DisInfo->StartX) * (DisInfo->EndY - DisInfo->StartY);
    LCD_Control(DisInfo->StartX, DisInfo->EndX, DisInfo->StartY, DisInfo->EndY, DisInfo->Direction);
    for(Count = 0; Count < Sum; Count++)
    {
        LCD_WR_Data(*pBuffer);
        pBuffer += 1;
    }
}

void LCD_Icon(LCD_CoordInfo *DisInfo, uint8_t *gBattery)
{
    uint16_t count;
    uint16_t Sum;
    uint8_t *p;
    p = (uint8_t*)gBattery;
    Sum = (DisInfo->EndX - DisInfo->StartX) * (DisInfo->EndY - DisInfo->StartY);
    LCD_Control(DisInfo->StartX, DisInfo->EndX, DisInfo->StartY, DisInfo->EndY, DisInfo->Direction);

    for(count = 0; count < Sum; count++)
    {
        LCD_WR_Data(D8toD16(&*p++, &*p++));
    }
}

bool getInfo(uint16_t *infoBuffer, Bitmap *info)
{
    uint8_t i = 0;
    uint16_t *pBuffer = (uint16_t*)infoBuffer;
    if((*infoBuffer & 0xff00) >> 8 == 0x00cc)
    {
        info->charCount = (uint8_t)(*(infoBuffer) & 0x00ff);

        pBuffer += 1;
        for(i = 0; i < info->charCount; )
        {
            info->bitMap[i++] = ((*(pBuffer)) & 0xff00) >> 8;
            info->bitMap[i++] = (*(pBuffer)) & 0x00ff;
            pBuffer += 1;

            //info->bitMap[i++] = *p;
            //p++;
        }
        info->realStartAddr = (uint32_t) & (*(pBuffer));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*6*12大小
在指定位置显示一个字符
x:0~234
y:0~308
num:要显示的字符*/
void LCD_ShowChar(uint8_t postion_x, uint16_t postion_y, uint32_t realStartAddr, uint16_t fColor, uint16_t bColor)
{
    uint8_t temp;
    uint8_t pos, t;
    uint16_t *pBuffer = (uint16_t*)(realStartAddr);
    LCD_Control(postion_x, postion_x + 15, postion_y, postion_y + 7, 'L');
    for(pos = 0; pos < 16; pos++)
    {
        if((pos % 2) == 0)
        {
            temp = (uint8_t)((*pBuffer & 0xff00) >> 8);
            for(t = 0; t < 8; t++)
            {
                if(temp & 0x80)LCD_WR_Data(fColor);
                else LCD_WR_Data(bColor);//白色
                temp <<= 1;
            }
        }
        else
        {
            temp = (uint8_t)(*pBuffer & 0x00ff);
            for(t = 0; t < 8; t++)
            {
                if(temp & 0x80)LCD_WR_Data(fColor);
                else LCD_WR_Data(bColor);//白色
                temp <<= 1;
            }
            pBuffer += 1;
        }
    }
}

void LCD_ShowChar_1624_ASCII(uint16_t postion_x, uint16_t postion_y, uint32_t realStartAddr, uint16_t fColor, uint16_t bColor)
{
    unsigned char ch;
    uint8_t pos, t;
    uint16_t baseAddr;
    uint16_t *pBuffer = (uint16_t*)(realStartAddr);
    baseAddr = (*pBuffer >> 8);
    baseAddr = 	(baseAddr - 32) * 48;
    LCD_Control(postion_x, postion_x + 15, postion_y - 23, postion_y, 'U');
    for(pos = 0; pos < 48; pos++)
    {
        ch = (nAsciiDot[baseAddr + pos]);
        if((pos % 2) == 0)
        {
            for(t = 0; t < 8; t++)
            {
                if((ch & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                ch <<= 1;
            }
        }
        else
        {
            for(t = 0; t < 8; t++)
            {
                if((ch & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                ch <<= 1;
            }
        }
    }
}

void LCD_DisplayGB1616(uint8_t postion_x, uint8_t  postion_y, unsigned char direction, uint32_t realStartAddr, uint16_t fColor, uint16_t bColor)
{
    uint16_t i, j;
    uint16_t *pBuffer = (uint16_t*)(realStartAddr);
    uint8_t m;
    LCD_Control(postion_x, postion_x + 15, postion_y - 15, postion_y , direction);
    for(i = 0; i < 32; i++)
    {
        if((i % 2) == 0)
        {
            m = (uint8_t)((*pBuffer & 0xff00) >> 8);
            for(j = 0; j < 8; j++)
            {
                if((m & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                m <<= 1;
            }
        }
        else
        {
            m = (uint8_t)(*pBuffer & 0x00ff);
            for(j = 0; j < 8; j++)
            {
                if((m & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                m <<= 1;
            }
            pBuffer += 1;
        }
    }
}

void LCD_DisplayGB2424(uint16_t postion_x, uint16_t  postion_y, unsigned char direction, uint32_t realStartAddr, uint16_t fColor, uint16_t bColor)
{
    uint16_t i, j;
    uint16_t *pBuffer = (uint16_t*)(realStartAddr);
    uint8_t m;
    LCD_Control(postion_x, postion_x + 23, postion_y - 23, postion_y, direction);
    for(i = 0; i < 72; i++)
    {
        if((i % 2) == 0)
        {
            m = (uint8_t)((*pBuffer & 0xff00) >> 8);
            for(j = 0; j < 8; j++)
            {
                if((m & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                m <<= 1;
            }
        }
        else
        {
            m = (uint8_t)(*pBuffer & 0x00ff);
            for(j = 0; j < 8; j++)
            {
                if((m & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                m <<= 1;
            }
            pBuffer += 1;
        }
    }
}

void LCD_CleanGB2424(uint16_t postion_x, uint16_t  postion_y, unsigned char direction, uint16_t bColor)
{
    uint16_t i, j;
    LCD_Control(postion_x, postion_x + 23, postion_y - 23, postion_y, direction);
    for(i = 0; i < 72; i++)
    {
		for(j = 0; j < 8; j++)
		{
			LCD_WR_Data(bColor);
		}
    }
}


void LCD_DisplayCharFromFlash(LCD_CoordInfo *DisInfo, FlashDataInfoData_16 *FlashDisData)
{
    uint16_t *pBuffer = (uint16_t*)(FlashDisData->StartAddr);
    uint8_t Count = 0;
    uint16_t startX, startY;
    Bitmap info;
    startX = DisInfo->StartX;
    startY = DisInfo->StartY;
    if(getInfo(pBuffer, &info))
    {
        for(Count = 0; Count < info.charCount; Count++)
        {
            if(info.bitMap[Count] == 0xdd)
            {
                LCD_ShowChar_1624_ASCII(startX, startY, info.realStartAddr, 0x0000, 0xffff);
                info.realStartAddr += 36;
                startX += 16;
            }
            else if(info.bitMap[Count] == 0xee)
            {
                LCD_DisplayGB2424(startX, startY, DisInfo->Direction, info.realStartAddr, 0x0000, 0xffff);
                info.realStartAddr += 72;
                startX += 24;
            }
        }
    }
}

void CleanLCD(LCD_CoordInfo *DisInfo, uint16_t bColor)
{
    uint32_t count;
    uint32_t Sum = 0;
    if((DisInfo->Direction == 'u') || (DisInfo->Direction == 'U'))
    {
//        LCD_Control(DisInfo->StartX, DisInfo->EndX, DisInfo->StartY - 23, DisInfo->EndY - 23, 'U');
        LCD_Control(DisInfo->StartX, DisInfo->EndX, DisInfo->StartY, DisInfo->EndY, 'U');
        Sum = (DisInfo->EndX - DisInfo->StartX) * (DisInfo->EndY - DisInfo->StartY);
    }
    if((DisInfo->Direction == 'L') || (DisInfo->Direction == 'l'))
    {
        LCD_Control(DisInfo->StartX, DisInfo->EndX, DisInfo->EndY, DisInfo->StartY, 'U');
        Sum = (DisInfo->EndX - DisInfo->StartX) * (DisInfo->StartY - DisInfo->EndY);
    }
    if((DisInfo->Direction == 'R') || (DisInfo->Direction == 'r'))
    {
        LCD_Control(DisInfo->EndX, DisInfo->StartX, DisInfo->StartY, DisInfo->EndY, 'U');
        Sum = (DisInfo->EndY - DisInfo->StartX) * (DisInfo->StartY - DisInfo->EndY);
    }
    if((DisInfo->Direction == 'D') || (DisInfo->Direction == 'd'))
    {
        LCD_Control(DisInfo->EndX, DisInfo->StartX, DisInfo->EndY, DisInfo->StartY, 'U');
        Sum = (DisInfo->StartX - DisInfo->EndX) * (DisInfo->StartY - DisInfo->EndY);
    }

    for(count = 0; count < Sum; count++)
    {
        LCD_WR_Data(bColor);
    }
}

void CleanCharFromLCD(LCD_CoordInfo *DisInfo, uint16_t bColor)
{        
    uint8_t Count = 0;
    uint16_t startX, startY;
    Bitmap info;
    startX = DisInfo->StartX;
    startY = DisInfo->StartY;
    for(Count = 0; Count < 8; Count++)
    {

		LCD_CleanGB2424(startX, startY, DisInfo->Direction, 0xffff);
		startX += 24;
    }
}


void LCD_DisplayPic(LCD_CoordInfo *DisInfo, FlashDataInfoData_16 *FlashDisData)
{
    uint16_t count;
    uint16_t Sum;
    Sum = (DisInfo->EndX - DisInfo->StartX) * (DisInfo->EndY - DisInfo->StartY);
    Sum = (Sum > FlashDisData->DataSize) ? (FlashDisData->DataSize) : (Sum); //在FlashDisData->DataSize 和屏幕显示区域中取较小的那个，防止溢出
    if(!DisInfo->Offset)
    {
        LCD_Control(DisInfo->StartX, DisInfo->EndX, DisInfo->StartY, DisInfo->EndY, 'U');
    }
    for(count = 0; count < Sum; count++)
    {
        LCD_WR_Data(FlashDisData->Data[count]);
        DisInfo->Offset++;
    }
}

void LCD_Battery(LCD_CoordInfo *DisInfo, uint8_t *gBattery)
{
    uint16_t count;
    uint16_t Sum;
    uint8_t *p;
    p = (uint8_t*)gBattery;
    Sum = (DisInfo->EndX - DisInfo->StartX) * (DisInfo->EndY - DisInfo->StartY);
    LCD_Control(DisInfo->StartX, DisInfo->EndX, DisInfo->StartY, DisInfo->EndY, 'U');
    for(count = 0; count < Sum; count++)
    {
        LCD_WR_Data(D8toD16(&*p++, &*p++));
    }
}

//显示数字
//x,y:起点坐标
//num:数值(0~65536);
void LCD_ShowNum(unsigned char postion_x, unsigned int postion_y, char Direction, unsigned long num, unsigned int fColor, unsigned int bColor)
{
    u32 res;
    u8 t = 0, t1 = 0;
    res = num;
    if(Direction == 'U')LCD_WR_CMD(0x0003, 0x10b0);
    if(Direction == 'D')LCD_WR_CMD(0x0003, 0x1080);
    if(Direction == 'L')LCD_WR_CMD(0x0003, 0x1098);
    if(Direction == 'R')LCD_WR_CMD(0x0003, 0x10a8);
    if(!num)TFT_ShowChar(postion_x, postion_y, '0', fColor, bColor); //显示0
    while(res)  //得到数字长度
    {
        res /= 10;
        t++;
    }
    t1 = t;
    while(t)    //显示数字
    {
        res = mypow(10, t - 1);
        if(Direction == 'U')
        {
            TFT_ShowChar(postion_x + (t1 - t) * 1, postion_y, (num / res) % 10 + '0', fColor, bColor);
        }
        else if(Direction == 'D')
        {
            TFT_ShowChar(postion_x , postion_y - (t1 - t) * 1, (num / res) % 10 + '0', fColor, bColor);
        }
        else if(Direction == 'L')
        {
            TFT_ShowChar(postion_x + (t1 - t) * 1, postion_y, (num / res) % 10 + '0', fColor, bColor);
        }
        else if(Direction == 'R')
        {
            TFT_ShowChar(postion_x + (t1 - t) * 1, postion_y + (t1 - t) * 1, (num / res) % 10 + '0', fColor, bColor);
        }
        t--;
    }
}

void LCD_ShowNum_1624_ASCII(uint16_t postion_x, uint16_t postion_y, uint8_t num, uint16_t fColor, uint16_t bColor)
{
    unsigned char ch;
    uint8_t pos, t;
    uint16_t baseAddr;
    baseAddr = 	(num + 16) * 48;
    LCD_Control(postion_x, postion_x + 15, postion_y - 23, postion_y , 'U');
    for(pos = 0; pos < 48; pos++)
    {
        ch = (nAsciiDot[baseAddr + pos]);
        if((pos % 2) == 0)
        {
            for(t = 0; t < 8; t++)
            {
                if((ch & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                ch <<= 1;
            }
        }
        else
        {
            for(t = 0; t < 8; t++)
            {
                if((ch & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                ch <<= 1;
            }
        }
    }
}

void TFT_ShowChar_1624_ASCII(uint16_t postion_x, uint16_t postion_y, unsigned char character, uint16_t fColor, uint16_t bColor)
{
    unsigned char ch;
    uint8_t pos, t;
    uint16_t baseAddr;
    baseAddr = 	(character - 32) * 48;
    LCD_Control(postion_x, postion_x + 15, postion_y - 23, postion_y , 'U');
    for(pos = 0; pos < 48; pos++)
    {
        ch = (nAsciiDot[baseAddr + pos]);
        if((pos % 2) == 0)
        {
            for(t = 0; t < 8; t++)
            {
                if((ch & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                ch <<= 1;
            }
        }
        else
        {
            for(t = 0; t < 8; t++)
            {
                if((ch & 0x80) == 0x80)
                {
                    LCD_WR_Data(fColor);
                }
                else
                {
                    LCD_WR_Data(bColor);
                }
                ch <<= 1;
            }
        }
    }
}

void DisplayGB2424(LCD_CoordInfo *DisInfo, signed char Code[2], unsigned int fColor, unsigned int bColor)
{
    unsigned int i, j, k;
	uint16_t postion_x = DisInfo->StartX  + (DisInfo->Offset)*24;
    uint16_t  postion_y = DisInfo->StartY;
unsigned char direction = DisInfo->Direction;
   LCD_Control(postion_x, postion_x + 23, postion_y - 23, postion_y, direction);
    for (k = 0; k < 64; k++) //64标示自建汉字库中的个数，循环查询内码
    {
        if ((GB_24[k].Index[0] == Code[0]) && (GB_24[k].Index[1] == Code[1]))
        {
            for(i = 0; i < 72; i++)
            {
                unsigned char m = GB_24[k].Msk[i];
                for(j = 0; j < 8; j++)
                {
                    if((m & 0x80) == 0x80)
                    {
                        LCD_WR_Data(fColor);
                    }
                    else
                    {
                        LCD_WR_Data(bColor);
                    }
                    m <<= 1;
                }
            }
        }
    }
}

void LCD_DisplayChinese(LCD_CoordInfo *DisInfo, signed char *s, unsigned int fColor, unsigned int bColor)
{
    DisInfo->Offset = 0;

	while(*s)
    {
        DisplayGB2424(DisInfo, s, fColor, bColor);
        s += 2;
		DisInfo->Offset++;
    }

}

/*******************************************************************************
* Function Name  : Time_Display
* Description    : Displays the current time.
* Input          : - TimeVar: RTC counter value.
* Output         : None
* Return         : None
*******************************************************************************/
void Time_Display(u32 TimeVar, LCD_CoordInfo *DisInfo)
{
    u32 THH = 0, TMM = 0, TSS = 0;
    /* Compute  hours */
    THH = TimeVar / 3600;
    /* Compute minutes */
    TMM = (TimeVar % 3600) / 60;
    /* Compute seconds */
    TSS = (TimeVar % 3600) % 60;
    //LCD_WR_CMD(0x0003, 0x10b0);
    //printf("\r\n Time: %0.2d:%0.2d:%0.2d\r",THH, TMM, TSS);

    LCD_ShowNum_1624_ASCII(DisInfo->StartX, DisInfo->StartY, THH / 10, 0x0000, 0xffff); //0
    LCD_ShowNum_1624_ASCII(DisInfo->StartX + 16, DisInfo->StartY, THH % 10, 0x0000, 0xffff); //数字串显示

    TFT_ShowChar_1624_ASCII(DisInfo->StartX + 32, DisInfo->StartY, ':', 0x0000, 0xffff); //单字符显示

    LCD_ShowNum_1624_ASCII(DisInfo->StartX + 48, DisInfo->StartY, TMM / 10, 0x0000, 0xffff); //0
    LCD_ShowNum_1624_ASCII(DisInfo->StartX + 64, DisInfo->StartY, TMM % 10, 0x0000, 0xffff); //数字串显示

    TFT_ShowChar_1624_ASCII(DisInfo->StartX + 80, DisInfo->StartY, ':', 0x0000, 0xffff); //单字符显示

    LCD_ShowNum_1624_ASCII(DisInfo->StartX + 96, DisInfo->StartY , TSS / 10, 0x0000, 0xffff); //0
    LCD_ShowNum_1624_ASCII(DisInfo->StartX + 112, DisInfo->StartY, TSS % 10, 0x0000, 0xffff); //数字串显示

}

