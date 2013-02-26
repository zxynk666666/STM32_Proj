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
#include "iButton.h"

/* Private define ------------------------------------------------------------*/
#define IBUTTON_IO	GPIOC
#define IBUTTON_PIN	GPIO_Pin_4

/* Private typedef -----------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/*one wire configuration*/
/*******************************************************************************************
这里我们假定使用通信端口IBUTTON_IO的第1位控制1-Wire总线。设定该位为0，将使1-Wire总线变为低电平；设定该位为1，1-Wire总线将被释放，此时1-Wire总线被电阻上拉，或被1-Wire从器件下拉。
    代码中的uDelay函数是一个用户编制的子程序，此函数用于产生一个1μs整数倍的延时。在不同的平台下，该函数的实现也是不同的。SetSpeed函数，用于设定标准速度和高速模式的延时时间。
*******************************************************************************************/

//IO输入配置
void GPIO_Conf_in(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = IBUTTON_PIN ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;//输入模式可能有问题！！！！！！！！
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
  //GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(IBUTTON_IO, &GPIO_InitStructure);
  GPIO_SetBits(IBUTTON_IO, IBUTTON_PIN);
}

//IO输出配置
void GPIO_Conf_out(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = IBUTTON_PIN ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(IBUTTON_IO, &GPIO_InitStructure);
}

//-----------------------------------------------------------------------------
// Generate a 1-Wire reset, return 1 if no presence detect was found,
// return 0 otherwise.
// (NOTE: Does not handle alarm presence from DS2404/DS1994)
//
uint8_t OWTouchReset(void)
{
        uint8_t result;

        uDelay(G);
		GPIO_Conf_out();
        GPIO_ResetBits(IBUTTON_IO,IBUTTON_PIN);// Drives DQ low
        uDelay(H);
        GPIO_SetBits(IBUTTON_IO, IBUTTON_PIN);// Releases the bus
        uDelay(I);
		 GPIO_Conf_in();
        result =  GPIO_ReadInputDataBit(IBUTTON_IO, IBUTTON_PIN)^ 0x01;  // Sample for presence pulse from slave
        uDelay(J); // Complete the reset sequence recovery
        return result; // Return sample presence pulse result
}

//-----------------------------------------------------------------------------
// Send a 1-Wire write bit. Provide 10us recovery time.
//
void OWWriteBit(uint8_t bit)
{
        GPIO_Conf_out();
        if (bit)
        {
                // Write '1' bit
                GPIO_ResetBits(IBUTTON_IO,IBUTTON_PIN);// Drives DQ low
                uDelay(A);
                GPIO_SetBits(IBUTTON_IO, IBUTTON_PIN);// Releases the bus
                uDelay(B); // Complete the time slot and 10us recovery
        }
        else
        {
                // Write '0' bit
                GPIO_ResetBits(IBUTTON_IO,IBUTTON_PIN);// Drives DQ low
                uDelay(C);
                GPIO_SetBits(IBUTTON_IO, IBUTTON_PIN);// Releases the bus
                uDelay(D);
        }
}

//-----------------------------------------------------------------------------
// Read a bit from the 1-Wire bus and return it. Provide 10us recovery time.
//
uint8_t OWReadBit(void)
{
        uint8_t result;
        GPIO_Conf_out();
        GPIO_ResetBits(IBUTTON_IO,IBUTTON_PIN);// Drives DQ low
        uDelay(A);
        GPIO_SetBits(IBUTTON_IO, IBUTTON_PIN);// Releases the bus
        uDelay(E);
        GPIO_Conf_in();
        result = GPIO_ReadInputDataBit(IBUTTON_IO, IBUTTON_PIN)& 0x01;// Sample the bit value from the slave
        uDelay(F); // Complete the time slot and 10us recovery

        return result;
}

//-----------------------------------------------------------------------------
// Write 1-Wire data byte
//
void OWWriteByte(uint8_t data)
{
        uint8_t loop;

        // Loop to write each bit in the byte, LS-bit first
        for (loop = 0; loop < 8; loop++)
        {
                OWWriteBit(data & 0x01);

                // shift the data byte for the next bit
                data >>= 1;
        }
}

//-----------------------------------------------------------------------------
// Read 1-Wire data byte and return it
//
uint8_t OWReadByte(void)
{
        uint8_t loop, result = 0;

        for (loop = 0; loop < 8; loop++)
        {
                // shift the result to get it ready for the next bit
                result >>= 1;

                // if result is one, then set MS bit
                if (OWReadBit())
                        result |= 0x80;
        }
        return result;
}


//-----------------------------------------------------------------------------
// Write a 1-Wire data byte and return the sampled result.
//
uint8_t OWTouchByte(uint8_t data)
{
        uint8_t loop, result=0;

        for (loop = 0; loop < 8; loop++)
        {
                // shift the result to get it ready for the next bit
                result >>= 1;

                // If sending a '1' then read a bit else write a '0'
                if (data & 0x01)
                {
                        if (OWReadBit())
                                result |= 0x80;
                }
                else
                        OWWriteBit(0);

                // shift the data byte for the next bit
                data >>= 1;
        }
        return result;
}

//-----------------------------------------------------------------------------
// Write a block 1-Wire data bytes and return the sampled result in the same
// buffer.
//
void OWBlock(unsigned char *data, uint8_t data_len)
{
        uint8_t loop;

        for (loop = 0; loop < data_len; loop++)
        {
                data[loop] = OWTouchByte(data[loop]);
        }
}

//-----------------------------------------------------------------------------
// Set all devices on 1-Wire to overdrive speed. Return '1' if at least one
// overdrive capable device is detected.
//
uint8_t OWOverdriveSkip(unsigned char *data, uint8_t data_len)
{
        // reset all devices
        if (OWTouchReset()) // Reset the 1-Wire bus
                return 0; // Return if no devices found

        // overdrive skip command
        OWWriteByte(0x3C);
        // do a 1-Wire reset in 'overdrive' and return presence result
        return OWTouchReset();
}
