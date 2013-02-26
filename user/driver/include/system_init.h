#include "stm32f10x.h"
#include "fsmc_sram.h"
#include "platform_config.h"

#define GPIO_INIT		0x01
#define RTC_INIT	 	0x02
#define USART_INIT		0x04
#define FSCM_INIT		0x08
#define FLASH_INIT		0x10
#define RCC_INIT		0x20
#define NVIC_INIT		0x40
#define SYSTICK_INIT	0x80
#define TIMER_INIT		0x0100

void RCC_Configuration(void);
void GPIO_Configuration(void);

/* NVIC configuration */
void NVIC_Configuration(void);
void RTC_Configuration(void);
void SystemResourcesInit(uint32_t Device_Init);
void Flash_Configuration(uint8_t eraseFlag);

