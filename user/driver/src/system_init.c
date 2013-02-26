#include "system_init.h"

ErrorStatus HSEStartUpStatus;
GPIO_InitTypeDef GPIO_InitStructure;

/**
  * @brief  Configures the SysTick.
  * @param  None
  * @retval : None
  */
void SysTick_Configuration(void)
{
	/* Setup SysTick Timer for 1 msec interrupts  */
	if (SysTick_Config((SystemFrequency) / 1000))
	{
		/* Capture error */
		while (1);
	}
	NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Configures the NVOC of USART.
  * @param  None
  * @retval : None
  */
USART_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the GPIO of USART.
  * @param  None
  * @retval : None
  */
USART_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //ʱ���ٶ�Ϊ50M
	/* USART Configuration  */
	/*����USART1(����1)����IO��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                   //���ͽ�ΪPA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                      //�����ϼ���������ʼ��PA��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                  //���ս�ΪPA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //���ÿ�©����
	GPIO_Init(GPIOA, &GPIO_InitStructure);                      //�����ϼ���������ʼ��PA��
}

/**
  * @brief  Configures the USART.
  * @param  None
  * @retval : None
  */
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	/* (1) ---- Enable USART1, GPIOA, �򿪴���1 ��GPIOAʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA , ENABLE);
	/* (2) ----  ����USART1�ж�*/
	USART_NVIC_Init();
	/* (3) ---- ����USART1(����1)����IO��*/
	USART_GPIO_Init();
	/* (4) ---- USART
	  - USART1 configured as follow:
	  - ������ = 115200 baud
	  - �ֳ� = 8 Bits
	  - һ��ֹͣλ
	  - ��У��λ
	  - �ر����������� (RTS and CTS signals)
	*/
	USART_InitStructure.USART_BaudRate = 115200;                   //���ô��ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    //�������ݳ���Ϊ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;         //����һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;           //��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//��ֹӲ��������ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�򿪴��ڵ��շ�����
	/* ����USART1 */
	USART_Init(USART1, &USART_InitStructure);                      //�����ϲ�����ʼ��USART1
	/* ���ô���1�����ж� */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                 //�򿪽����ж�
	/*ʹ�ܴ���1 */
	USART_Cmd(USART1, ENABLE);                                     //�򿪴���1
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

/**
  * @brief  Configures the GPIO of FSCM.
  * @param  None
  * @retval : None
  */
void FSCM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //ʱ���ٶ�Ϊ50M
	/* FSCM Configuration  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                 //LCD-RST
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                 //LCD-RST
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
								  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
								  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
	  PE.14(D11), PE.15(D12) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
								  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
								  GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/* NE1 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* RS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_7);          //CS=1
	GPIO_SetBits(GPIOD, GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_1);
	GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);        //RESET=0
	GPIO_SetBits(GPIOD, GPIO_Pin_4);          //RD=1
	GPIO_SetBits(GPIOD, GPIO_Pin_5);          //WR=1
	GPIO_SetBits(GPIOD, GPIO_Pin_13);
}

/**
  * @brief  Configures the FSCM.
  * @param  None
  * @retval : None
  */
void FSCM_Configuration(void)
{
	/*����FSCM����IO��*/
	FSCM_GPIO_Init();
	/* Enable the FSMC Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval : None
  */
void RTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	/* Reset Backup Domain */
	BKP_DeInit();
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{} 
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval : None
  */
void NVIC_Configuration(void)
{
}


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();                                             //ʱ�ӿ��ƼĴ���ȫ���ָ�Ĭ��ֵ
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);                                //�ⲿ����ʱ��Դ������8M����
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();               //�ȴ��ⲿʱ�Ӿ���
	if(HSEStartUpStatus == SUCCESS)                           //���ʱ�������ɹ�
	{
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);                        //����AHB�豸ʱ��Ϊϵͳʱ��1��Ƶ
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);                         //����APB2�豸ʱ��ΪHCLKʱ��1��Ƶ
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);                         //����APB1�豸ʱ��ΪHCLKʱ��2��Ƶ
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);                      //�趨�ڲ�FLASH�ĵ���ʱ����Ϊ2����
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   //ʹ��FLASHԤ��ȡ������
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);    //����PLLʱ��Ϊ�ⲿ����ʱ�ӵ�9��Ƶ��8MHz * 9 = 72 MHz
		/* Enable PLL */
		RCC_PLLCmd(ENABLE);                                     //ʹ��PLLʱ��
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      //�ȴ�PLLʱ���������׼������
		{
		}
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);              //ʹ��PLLʱ����Ϊϵͳʱ��Դ
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)                    //����ϵͳ����ʱ��Դȷ��Ϊ�ⲿ���پ���8M����
		{
		}
	}
}

void GPIO_NVIC_Init(void)
{

}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval : None
  */
void GPIO_Configuration(void)
{
	/* (1) ---- Enable ,��GPIOA GPIOCʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
						   | RCC_APB2Periph_GPIOB 
						   | RCC_APB2Periph_GPIOC 
						   | RCC_APB2Periph_GPIOD
						   | RCC_APB2Periph_GPIOE
						   , ENABLE);
	/* (2) ----  ����GPIO�ж�*/
	GPIO_NVIC_Init();
	
	/* (3) ---- ���� iButton ����IO��*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //ʱ���ٶ�Ϊ50M
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                   //���ͽ�ΪPC4
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //���ÿ�©����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_Out_OD;                 
	GPIO_Init(GPIOC, &GPIO_InitStructure);                      //�����ϼ���������ʼ��PA��
	
	/* (4 - PA0 PA1) ---- ���� ���״̬��� ����IO��*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //ʱ���ٶ�Ϊ50M
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;                   
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //���ÿ�©����
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                      //�����ϼ���������ʼ��PA��
	
	/* (4 - PE4 PE5) ---- ���� ���״̬��� ����IO��*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //ʱ���ٶ�Ϊ50M
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;                   
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //���ÿ�©����
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);                      //�����ϼ���������ʼ��PA��

	/* (5) ---- ���� �� ����IO��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* (6) ---- ���� ���״̬LEDָʾ�� ����IO��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  	//����
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 	//���
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//�Ƴ�
  	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/*  (7) ---- ����LCD  ��������IO��--PC7,PC8  */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //ʱ���ٶ�Ϊ50M
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;                 
	GPIO_Init(GPIOC, &GPIO_InitStructure);                      //�����ϼ���������ʼ��PA��
	
}

void Flash_Configuration(uint8_t eraseFlag)
{
	__IO uint32_t StartAddr = 0x08040000;
	__IO uint32_t EndAddr = 0x0805AB38;
	uint32_t EraseCounter;
	__IO uint32_t NbrOfPage;
	volatile FLASH_Status FLASHStatus;
	FLASHStatus = FLASH_COMPLETE;
	/* Unlock the Flash Program Erase controller */
	FLASH_Unlock();
	/* Define the number of page to be erased */
	NbrOfPage = (EndAddr - StartAddr) / FLASH_PAGE_SIZE;
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	if(eraseFlag == TRUE)
	{
		/* Erase the FLASH pages */
		for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
		{
			FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * EraseCounter));
		}
	}
}

void Timer_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 1;                 //�Զ�װ��
	TIM_TimeBaseStructure.TIM_Prescaler = 72;       //72M��Ƶ�ʵ�1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;   
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //���¼���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
}

/**
  * @brief  Initialize the correspond device.
  * @param Device_Init: specifies the devices which will be initalized
  *   This parameter can be any combination of the following values:
  * @arg GPIO_INIT, RTC_INIT, USART_INIT, FLASH_INIT,
  *   FSCM_INIT, RCC_INIT
  * @retval : None
  */
void SystemResourcesInit(uint32_t Device_Init)
{
	if(Device_Init & RCC_INIT)
	{
		/* System Clocks Configuration */
		RCC_Configuration();
	}
	if(Device_Init & FSCM_INIT)
	{
		FSCM_Configuration();
	}
	if(Device_Init & FLASH_INIT)
	{
		Flash_Configuration(FALSE);
	}
	if(Device_Init & NVIC_INIT)
	{
		/* NVIC configuration */
		NVIC_Configuration();
	}
	if(Device_Init & GPIO_INIT)
	{
		/* Configure the GPIO */
		GPIO_Configuration();
	}
	if(Device_Init & USART_INIT)
	{
		/* Configure the USART */
		USART_Configuration();
	}
	if(Device_Init & SYSTICK_INIT)
	{
		/* SysTick Configuration */
		SysTick_Configuration();
	}
	if(Device_Init & TIMER_INIT)
	{
		/* SysTick Configuration */
		Timer_Configuration();
	}
	
}
