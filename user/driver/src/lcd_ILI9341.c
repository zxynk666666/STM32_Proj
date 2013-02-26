#include "fsmc_sram.h"							   //调用的库文件
#include <math.h>
#include "font.h"

//#include <pic.c>
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp cmd ADDR

unsigned long color1=0;
extern uint32_t tCounter;
//void MUC_Init();
void LCD_Init(void);										//初始化LCD
void LCD_WR_CMD_8(unsigned int val);
void LCD_WR_CMD(unsigned int val,unsigned int n);
void LCD_WR_Data_8(unsigned char val);
void LCD_WR_Data(unsigned int val);
void LCD_Test(unsigned char a);
void LCD_WR_REG(unsigned int index);
void LCD_Rest(void);
void LCD_Control(uint16_t StartX, uint16_t EndX, uint16_t StartY, uint16_t EndY, unsigned char direction);

//演示程序
void LCD_Test(unsigned char a)
{
	unsigned long n;

		n=0;
		//初始化屏幕起始坐标
		LCD_Control(100, 149, 100, 149, 'U');
		while(n < 700)
		{
			LCD_WR_Data(a);
			n++;
	 	}
}


void LCD_Control(uint16_t StartX, uint16_t EndX, uint16_t StartY, uint16_t EndY, unsigned char direction)
{
	LCD_WR_CMD_8(0X36);
	
	if((direction == 'L') || (direction == 'l'))
	{
		LCD_WR_Data_8(0x08);
		//初始化屏幕起始坐标
		LCD_WR_CMD_8(0X2A); 
		LCD_WR_Data_8((uint8_t)((239 - EndX) >> 8));
		LCD_WR_Data_8((uint8_t)((239 - EndX) & 0xff));
		
		LCD_WR_Data_8((uint8_t)((239 - StartX) >> 8));
		LCD_WR_Data_8((uint8_t)((239 - StartX) & 0xff));

		LCD_WR_CMD_8(0X2B); 
		LCD_WR_Data_8((uint8_t)(StartY >> 8));
		LCD_WR_Data_8((uint8_t)(StartY & 0xff));
			
		LCD_WR_Data_8((uint8_t)(EndY >> 8));
		LCD_WR_Data_8((uint8_t)(EndY & 0xff));
	} else if((direction == 'R') || (direction == 'r'))
	{
		LCD_WR_Data_8(0xC8);
		//初始化屏幕起始坐标
		LCD_WR_CMD_8(0X2A); 			
		LCD_WR_Data_8((uint8_t)(StartX >> 8));
		LCD_WR_Data_8((uint8_t)(StartX & 0xff));

		LCD_WR_Data_8((uint8_t)(EndX >> 8));
		LCD_WR_Data_8((uint8_t)(EndX & 0xff));

		LCD_WR_CMD_8(0X2B); 		
		LCD_WR_Data_8((uint8_t)((319 - EndY) >> 8));
		LCD_WR_Data_8((uint8_t)((319 - EndY) & 0xff));
		
		LCD_WR_Data_8((uint8_t)((319 - StartY) >> 8));
		LCD_WR_Data_8((uint8_t)((319 - StartY) & 0xff));
	} else if((direction == 'D') || (direction == 'd'))
	{
		LCD_WR_Data_8(0x28);
		//初始化屏幕起始坐标
		LCD_WR_CMD_8(0X2A); 
		LCD_WR_Data_8((uint8_t)(StartX >> 8));
		LCD_WR_Data_8((uint8_t)(StartX & 0xff));

		LCD_WR_Data_8((uint8_t)(EndX >> 8));
		LCD_WR_Data_8((uint8_t)(EndX & 0xff));

		LCD_WR_CMD_8(0X2B); 	
		LCD_WR_Data_8((uint8_t)((239 - EndY) >> 8));
		LCD_WR_Data_8((uint8_t)((239 - EndY) & 0xff));
		
		LCD_WR_Data_8((uint8_t)((239 - StartY) >> 8));
		LCD_WR_Data_8((uint8_t)((239 - StartY) & 0xff));

	} else if((direction == 'U') || (direction == 'u'))
	{
		LCD_WR_Data_8(0x68);
		//初始化屏幕起始坐标
		LCD_WR_CMD_8(0X2A); 
		LCD_WR_Data_8((uint8_t)(StartX >> 8));
		LCD_WR_Data_8((uint8_t)(StartX & 0xff));
		
		LCD_WR_Data_8((uint8_t)(EndX >> 8));
		LCD_WR_Data_8((uint8_t)(EndX & 0xff));		
		
		LCD_WR_CMD_8(0X2B); 	
		LCD_WR_Data_8((uint8_t)(StartY >> 8));
		LCD_WR_Data_8((uint8_t)(StartY & 0xff));
		
		LCD_WR_Data_8((uint8_t)(EndY >> 8));
		LCD_WR_Data_8((uint8_t)(EndY & 0xff));
	}
	LCD_WR_CMD_8(0x2C);
}

//写寄存器地址函数
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint8_t *) (Bank1_LCD_C)= (uint8_t)index; 	
	*(__IO uint8_t *) (Bank1_LCD_C)= (uint8_t)(index >> 8); 	
}

void LCD_Rest(void)
{
	GPIO_SetBits(GPIOE, GPIO_Pin_1 );
	delay(1);
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	delay(10);
	GPIO_SetBits(GPIOE, GPIO_Pin_1 );
	delay(120);
}


//写cmd函数
//输入：dbw 数据位数，1为16位，0为8位。
void LCD_WR_CMD_8(unsigned int val)
{	
	*(__IO uint8_t *) (Bank1_LCD_C)= (uint8_t)val;	
}

//写cmd函数
//输入：dbw 数据位数，1为16位，0为8位。
void LCD_WR_CMD(unsigned int val,unsigned int n)
{	
	*(__IO uint8_t *) (Bank1_LCD_C) = val;	
}

//写8位data函数 初始化
void    LCD_WR_Data_8(unsigned char val)
{   
	*(__IO uint8_t *) (Bank1_LCD_D) = (uint8_t)val; 		
}

//写8位data函数
void    LCD_WR_Data(unsigned int val)
{   	
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 
}

uint8_t LCD_RD_Data(void)
{
	return *(__IO uint8_t *) (Bank1_LCD_D);
}

//初始化函数
void LCD_Init(void)
{
	LCD_Rest();	 
	//************* Start Initial Sequence **********// 
	 
	LCD_WR_CMD_8(0xCF);  
	LCD_WR_Data_8(0x00); 
	LCD_WR_Data_8(0x81); 
	LCD_WR_Data_8(0X30); 
 
  	LCD_WR_CMD_8(0xED);  
	LCD_WR_Data_8(0x64); 
	LCD_WR_Data_8(0x03); 
	LCD_WR_Data_8(0X12); 
	LCD_WR_Data_8(0X81); 
	 
	LCD_WR_CMD_8(0xE8);  
	LCD_WR_Data_8(0x85); 
	LCD_WR_Data_8(0x11); 
	LCD_WR_Data_8(0x78); 


	LCD_WR_CMD_8(0xCB);  
	LCD_WR_Data_8(0x39); 
	LCD_WR_Data_8(0x2C); 
	LCD_WR_Data_8(0x00); 
	LCD_WR_Data_8(0x34); 
	LCD_WR_Data_8(0x02); 
	 
	LCD_WR_CMD_8(0xF7);  
	LCD_WR_Data_8(0x20); 
	 
	LCD_WR_CMD_8(0xEA);  
	LCD_WR_Data_8(0x00); 
	LCD_WR_Data_8(0x00); 
	 

	LCD_WR_CMD_8(0xb6);  
	LCD_WR_Data_8(0x0a); 
	LCD_WR_Data_8(0xa2); 

	LCD_WR_CMD_8(0xC0);       //Power control 
	LCD_WR_Data_8(0x21);     //VRH[5:0] 
	 
	LCD_WR_CMD_8(0xC1);       //Power control 
	LCD_WR_Data_8(0x11);     //SAP[2:0];BT[3:0] 
	 
	LCD_WR_CMD_8(0xC5);       //VCM control 
	//LCD_WR_Data_8(0x33); 
	//LCD_WR_Data_8(0x3C); 
	LCD_WR_Data_8(0x45);
	LCD_WR_Data_8(0x45);
	
	LCD_WR_CMD_8(0xC7);       //VCM control2 
	//LCD_WR_Data_8(0xc0); 
	LCD_WR_Data_8(0xa2); 
	
	LCD_WR_CMD_8(0x36);       // Memory Access Control 
	//LCD_WR_Data_8(0x48); 
	LCD_WR_Data_8(0xc8);
	
	LCD_WR_CMD_8(0x3A);       
	LCD_WR_Data_8(0x55); 

	LCD_WR_CMD_8(0xB1);       //VCM control 
	LCD_WR_Data_8(0x00); 
	LCD_WR_Data_8(0x18); 

	 
	LCD_WR_CMD_8(0xF2);       // 3Gamma Function Disable 
	LCD_WR_Data_8(0x00); 
	 
	LCD_WR_CMD_8(0x26);       //Gamma curve selected 
	LCD_WR_Data_8(0x01); 
	 
	LCD_WR_CMD_8(0xE0);       //Set Gamma 
	LCD_WR_Data_8(0x0F); 
	LCD_WR_Data_8(0x1a); 
	LCD_WR_Data_8(0x18); 
	LCD_WR_Data_8(0x0c); 
	LCD_WR_Data_8(0x10); 
	LCD_WR_Data_8(0x08); 
	LCD_WR_Data_8(0x45); 
	LCD_WR_Data_8(0xba); 
	LCD_WR_Data_8(0x30); 
	LCD_WR_Data_8(0x02); 
	LCD_WR_Data_8(0x00); 
	LCD_WR_Data_8(0x00); 
	LCD_WR_Data_8(0x00); 
	LCD_WR_Data_8(0x00); 
	LCD_WR_Data_8(0x00); 
	 
	LCD_WR_CMD_8(0XE1);       //Set Gamma 
	LCD_WR_Data_8(0x00); 
	LCD_WR_Data_8(0x24); 
	LCD_WR_Data_8(0x27); 
	LCD_WR_Data_8(0x03); 
	LCD_WR_Data_8(0x0e); 
	LCD_WR_Data_8(0x06); 
	LCD_WR_Data_8(0x3a); 
	LCD_WR_Data_8(0x45); 
	LCD_WR_Data_8(0x4f); 
	LCD_WR_Data_8(0x0d); 
	LCD_WR_Data_8(0x1f); 
	LCD_WR_Data_8(0x0f); 
	LCD_WR_Data_8(0x3f); 
	LCD_WR_Data_8(0x3f); 
	LCD_WR_Data_8(0x0F); 

	/*初始化屏幕起始坐标
	LCD_WR_CMD_8(0X2A); 
	LCD_WR_Data_8(0x00);
	LCD_WR_Data_8(0x00);

	LCD_WR_CMD_8(0X2B); 
	LCD_WR_Data_8(0x00);
	LCD_WR_Data_8(0x00);
	*/
	
	delay(50);
	LCD_WR_CMD_8(0x11);       //Exit Sleep 
	delay(120); 
	LCD_WR_CMD_8(0x29);       //Display on 	
}


//m^n函数
unsigned long mypow(unsigned char m,unsigned char n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/*6*12大小
在指定位置显示一个字符
x:0~234
y:0~308
num:要显示的字符*/
void TFT_ShowChar(unsigned char postion_x,unsigned int postion_y,unsigned char num,unsigned int fColor, unsigned int bColor)
{       

    unsigned char temp;
    unsigned char pos,t;      

   	LCD_WR_CMD(0x0050,postion_x*8); 	//x start point
	LCD_WR_CMD(0x0051,postion_x*8+7); 	//y start point
	LCD_WR_CMD(0x0052,postion_y*16);	//x end point
    LCD_WR_CMD(0x0053,postion_y*16+15);	//y end point
	
	LCD_WR_CMD(0x0020,postion_x*8);   // GRAM horizontal Address  X		 区域RAM位置
	LCD_WR_CMD(0x0021,postion_y*16);   // GRAM Vertical Address	 Y   
	LCD_WR_REG(0x0022);
	
	num=num-' ';//得到偏移后的值
	for(pos=0;pos<16;pos++)
	{
	    temp=asc2_1206[num][pos];
	    for(t=0;t<8;t++)
	    {                 
	        if(temp&0x80)LCD_WR_Data(fColor);
	        else LCD_WR_Data(bColor);//白色    
	        temp<<=1; 
	    }
	}
}

unsigned int D8toD16(unsigned char *Dl,unsigned char *Dh)       
{	
   unsigned int tmp=0x0000;
   tmp=*Dh;
   tmp<<=8;
   tmp=tmp+*Dl;
   return tmp;
}
