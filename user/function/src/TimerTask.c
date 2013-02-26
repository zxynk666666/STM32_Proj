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
#include "TimerTask.h"

/* Private define ------------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/
extern TMR       TmrTbl[TMR_MAX_TMR];          /* Table of timers managed by this module             */

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/*
*********************************************************************************************************
*                                   CONFIGURE TIMER TIMEOUT FUNCTION
*
* Description : Set the user defined function when the timer expires.
* Arguments   : n         is the timer number 0..TMR_MAX_TMR-1
*               fnct      is a pointer to a function that will be executed when the timer expires
*               arg       is a pointer to an argument that is passed to 'fnct'
* Returns     : None.
*********************************************************************************************************
*/

void   TmrCfgFnct(bool TmrRep, uint8_t n, void (*fnct)(void *), void *arg)
{
	TMR *ptmr;
	if (n < TMR_MAX_TMR)
	{
		ptmr             = &TmrTbl[n];
		ptmr->TmrFnct    = fnct;                 /* Store pointer to user function into timer          */
		ptmr->TmrFnctArg = arg;                  /* Store user's function arguments pointer            */
		ptmr->TmrRep = TmrRep;
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             CHECK TIMER
*
* Description : This function checks to see if a timer has expired
* Arguments   : n      is the timer to check
* Returns     : 0      if the timer has expired
*               TmrCtr the remaining time before the timer expires in 1/10 second
*********************************************************************************************************
*/

uint16_t  TmrChk (uint8_t n)
{
	uint16_t val;
	val = 0;
	if (n < TMR_MAX_TMR)
	{
		val = TmrTbl[n].TmrCtr;
	}
	return (val);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                       FORMAT TIMER INTO STRING
*
* Description : Formats a timer into an ASCII string.
* Arguments   : n      is the desired timer
*               s      is a pointer to the destination string.  The destination string must be large
*                      enough to hold the formatted timer value which will have the following format:
*                          "MM:SS.T"
*********************************************************************************************************
*/

void  TmrFormat (uint8_t n, char *s)
{
	uint8_t   min;
	uint8_t   sec;
	uint8_t   tenths;
	uint16_t  val;
	if (n < TMR_MAX_TMR)
	{
		val    = TmrTbl[n].TmrCtr;                    /* Get local copy of timer for conversion        */
		min    = (uint8_t)(val / 600);
		sec    = (uint8_t)((val - min * 600) / 10);
		tenths = (uint8_t)(val % 10);
		s[0]   = min / 10 + '0';                      /* Convert TIMER to ASCII                        */
		s[1]   = min % 10 + '0';
		s[2]   = ':';
		s[3]   = sec / 10 + '0';
		s[4]   = sec % 10 + '0';
		s[5]   = '.';
		s[6]   = tenths   + '0';
		s[7]   = 0;
	}
}

void  TmrInit (void)
{

	uint8_t  i;
	TMR   *ptmr;
	ptmr = &TmrTbl[0];
	for (i = 0; i < TMR_MAX_TMR; i++)            /* Clear and disable all timers                       */
	{
		ptmr->TmrEn   = FALSE;
		ptmr->TmrOK   = FALSE;
		ptmr->TmrCtr  = 0;
		ptmr->TmrInit = 0;
		ptmr->TmrFnct = 0;
		ptmr++;
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             RESET TIMER
*
* Description : This function reloads a timer with its initial value
* Arguments   : n         is the timer to reset
* Returns     : None.
*********************************************************************************************************
*/

void  TmrReset (uint8_t n)
{
	TMR *ptmr;
	if (n < TMR_MAX_TMR)
	{
		ptmr         = &TmrTbl[n];
		ptmr->TmrCtr = ptmr->TmrInit;       /* Reload the counter                                      */
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                        SET TIMER (SPECIFYING MINUTES, SECONDS and TENTHS)
*
* Description : Set the timer with the specified number of minutes, seconds and 1/10 seconds.  The
*               function converts the minutes, seconds and tenths into tenths.
* Arguments   : n         is the timer number 0..TMR_MAX_TMR-1
*               min       is the number of minutes
*               sec       is the number of seconds
*               tenths    is the number of tenths of a second
* Returns     : None.
*********************************************************************************************************
*/

void  TmrSetMST (uint8_t n, uint8_t min, uint8_t sec, uint8_t tenths)
{
	TMR   *ptmr;
	uint16_t  val;
	if (n < TMR_MAX_TMR)
	{
		ptmr          = &TmrTbl[n];
		val           = (uint16_t)min * 600 + (uint16_t)sec * 10 + (uint16_t)tenths;
		ptmr->TmrInit = val * 100;
		ptmr->TmrCtr  = val * 100;
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                               SET TIMER (SPECIFYING TENTHS OF SECOND)
*
* Description : Set the timer with the specified number of 1/10 seconds.
* Arguments   : n         is the timer number 0..TMR_MAX_TMR-1
*               tenths    is the number of 1/10 second to load into the timer
* Returns     : None.
*********************************************************************************************************
*/

void  TmrSetT (uint8_t n, uint16_t tenths)
{
	TMR *ptmr;
	if (n < TMR_MAX_TMR)
	{
		ptmr          = &TmrTbl[n];
		ptmr->TmrInit = tenths;
		ptmr->TmrCtr  = tenths;
	}
}


/*
*********************************************************************************************************
*                                             START TIMER
*
* Description : This function start a timer
* Arguments   : n         is the timer to start
* Returns     : None.
*********************************************************************************************************
*/

void  TmrStart (uint8_t n)
{
	if (n < TMR_MAX_TMR)
	{
		TmrTbl[n].TmrEn = TRUE;
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             STOP TIMER
*
* Description : This function stops a timer
* Arguments   : n         is the timer to stop
* Returns     : None.
*********************************************************************************************************
*/

void  TmrStop (uint8_t n)
{
	if (n < TMR_MAX_TMR)
	{
		TmrTbl[n].TmrEn = FALSE;
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          TIMER MANAGER TASK
*
* Description : This task is created by TmrInit() and is responsible for updating the timers.
*               TmrTask() executes every 1/10 of a second.
* Arguments   : None.
* Returns     : None.
* Note(s)     : 1) The function to execute when a timer times out is executed outside the critical
*                  section.
*********************************************************************************************************
*/

void  TmrTask (void *data)
{
	TMR    *ptmr;
	uint8_t   i;
	void  (*pfnct)(void *);                           /* Function to execute when timer times out      */
	void   *parg;                                     /* Arguments to pass to above function           */
	data  = data;                                     /* Avoid compiler warning (uC/OS-II req.)        */
	pfnct = (void (*)(void *))0;                      /* Start off with no function to execute         */
	parg  = (void *)0;
	//for (;;)
	{
		ptmr = &TmrTbl[0];                            /* Point at beginning of timer table             */		
		for (i = 0; i < TMR_MAX_TMR; i++)
		{			
			if (ptmr->TmrEn == TRUE)                  /* Decrement timer only if it is enabled         */
			{
				if (ptmr->TmrOK == TRUE)
				{
					pfnct       = ptmr->TmrFnct;       /* Get pointer to function to execute ...   */
					parg        = ptmr->TmrFnctArg;    /* ... and its argument                     */
					if (pfnct != (void (*)(void *))0)         /* See if we need to execute function for ...    */
					{
						(*pfnct)(parg);                       /* ... timed out timer.                          */
						pfnct = (void (*)(void *))0;
						ptmr->TmrOK = FALSE;
						TmrSetT(i, ptmr->TmrInit);
						TmrStop(i);
						if(ptmr->TmrRep == TRUE)
						{
							TmrStart(i);
						}
					}
				}
			}
			ptmr++;
		}
	}
}

void TmrDiDa(void)
{
	TMR    *ptmr;
	uint8_t   i;
	ptmr = &TmrTbl[0];                            /* Point at beginning of timer table             */
	for (i = 0; i < TMR_MAX_TMR; i++)
	{
		if (ptmr->TmrEn == TRUE)                  /* Decrement timer only if it is enabled         */
		{
			if (ptmr->TmrCtr > 0)
			{
				ptmr->TmrCtr--;
				if (ptmr->TmrCtr == 0)                 /* See if timer expired                     */
				{
					ptmr->TmrOK = TRUE;
				}
			}
		}
		ptmr++;
	}
}
