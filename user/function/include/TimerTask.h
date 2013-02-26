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
#define  TMR_MAX_TMR          20

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
typedef struct tmr                               /* TIMER DATA STRUCTURE                               */
{
	bool   TmrEn;                             /* Flag indicating whether timer is enabled           */
	bool   TmrOK;
	bool   TmrRep;
	uint32_t    TmrCtr;                            /* Current value of timer (counting down)             */
	uint32_t    TmrInit;                           /* Initial value of timer (i.e. when timer is set)    */
	void    (*TmrFnct)(void *);                  /* Function to execute when timer times out           */
	void     *TmrFnctArg;                        /* Arguments supplied to user defined function        */
} TMR;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void   TmrCfgFnct(bool TmrRep, uint8_t n, void (*fnct)(void *), void *arg);
uint16_t  TmrChk (uint8_t n);
void  TmrFormat (uint8_t n, char *s);
void  TmrInit (void);
void  TmrReset (uint8_t n);
void  TmrSetMST (uint8_t n, uint8_t min, uint8_t sec, uint8_t tenths);
void  TmrSetT (uint8_t n, uint16_t tenths);
void  TmrStart (uint8_t n);
void  TmrStop (uint8_t n);
void  TmrTask (void *data);
void TmrDiDa(void);

