#include "fsmc_sram.h"							   //调用的库文件
#include <math.h>
#include "font.h"

//#include <pic.c>
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR

unsigned long color1=0;
//void MUC_Init();
void LCD_Init(void);										//初始化LCD
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);

void LCD_WR_Data(unsigned int val);
void LCD_Test(unsigned int a);
void ini(void);

void LCD_WR_ZF(unsigned int a, unsigned int b, unsigned int a1,unsigned int b1, unsigned int d,unsigned int e, unsigned char g, unsigned char *f); 
void LCD_WR_Pixel(unsigned int a, unsigned int b, unsigned int e) ;
unsigned char *NUM_Pub(unsigned int a);

unsigned int color[]={0xf800,0x07e0,0x001f,0xffe0,0x0000,0xffff,0x07ff,0xf81f};

void LCD_Rest(void);

void LCD_Control(uint16_t StartX, uint16_t EndX, uint16_t StartY, uint16_t EndY, unsigned char direction)
{
	
	if(direction=='U')LCD_WR_CMD(0x0003, 0x10b0);
	if(direction=='D')LCD_WR_CMD(0x0003, 0x1080);
	if(direction=='L')LCD_WR_CMD(0x0003, 0x1098);
	if(direction=='R')LCD_WR_CMD(0x0003, 0x10a8);

	LCD_WR_CMD(0x0050, StartX);  // Horizontal GRAM Start Address           区域
	LCD_WR_CMD(0x0051, EndX);  // Horizontal GRAM End Address
	LCD_WR_CMD(0x0052, StartY);  // Vertical GRAM Start Address
	LCD_WR_CMD(0x0053, EndY);  // Vertical GRAM End Address
	LCD_WR_CMD(0x0020, StartX);   // GRAM horizontal Address  X      区域RAM位置
	LCD_WR_CMD(0x0021, StartY);   // GRAM Vertical Address   Y
	LCD_WR_REG(0x0022);
}

void LCD_Rest(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	delay(0xffff);
	GPIO_SetBits(GPIOE, GPIO_Pin_1 );
	delay(0xffff);
}

//写寄存器地址函数
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}

//写寄存器数据函数
//输入：dbw 数据位数，1为16位，0为8位。
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}



//写16位数据函数
void    LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}


//初始化函数
void LCD_Init(void)
{
LCD_Rest();	 


	LCD_WR_CMD(0x00E3, 0x3008); // Set internal timing
	LCD_WR_CMD(0x00E7, 0x0012); // Set internal timing
	LCD_WR_CMD(0x00EF, 0x1231); // Set internal timing
	LCD_WR_CMD(0x0000, 0x0100); // Start Oscillation
	LCD_WR_CMD(0x0001, 0x0100); // set SS and SM bit
	LCD_WR_CMD(0x0002, 0x0700); // set 1 line inversion

	LCD_WR_CMD(0x0003, 0x10b0); // set GRAM write direction and BGR=0,262K colors,1 transfers/pixel.
	LCD_WR_CMD(0x0004, 0x0000); // Resize register
	LCD_WR_CMD(0x0008, 0x0202); // set the back porch and front porch
	LCD_WR_CMD(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	LCD_WR_CMD(0x000A, 0x0001); // FMARK function
	LCD_WR_CMD(0x000C, 0x0000); // RGB interface setting
	LCD_WR_CMD(0x000D, 0x0000); // Frame marker Position
	LCD_WR_CMD(0x000F, 0x0000); // RGB interface polarity
//Power On sequence 
	LCD_WR_CMD(0x0010, 0x1590); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_WR_CMD(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_WR_CMD(0x0012, 0x0000); // VREG1OUT voltage
	LCD_WR_CMD(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	delay(200); // Dis-charge capacitor power voltage
	LCD_WR_CMD(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_WR_CMD(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
	delay(50); // delay 50ms
	LCD_WR_CMD(0x0012, 0x001C); // External reference voltage= Vci;
	delay(50); // delay 50ms
	LCD_WR_CMD(0x0013, 0x1800); // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
	LCD_WR_CMD(0x0029, 0x001C); // R29=000C when R12=009D;VCM[5:0] for VCOMH
	LCD_WR_CMD(0x002B, 0x000D); // Frame Rate = 91Hz
	delay(50); // delay 50ms
	LCD_WR_CMD(0x0020, 0x0000); // GRAM horizontal Address
	LCD_WR_CMD(0x0021, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//

  LCD_WR_CMD(0x0030, 0x0203);
  LCD_WR_CMD(0x0031, 0x080F);
  LCD_WR_CMD(0x0032, 0x0401);
  LCD_WR_CMD(0x0033, 0x050B);
  LCD_WR_CMD(0x0034, 0x3330);
  LCD_WR_CMD(0x0035, 0x0B05);
  LCD_WR_CMD(0x0036, 0x0005);
  LCD_WR_CMD(0x0037, 0x0F08);
  LCD_WR_CMD(0x0038, 0x0302);
  LCD_WR_CMD(0x0039, 0x3033);
//---------------- Set GRAM area ---------------//
	LCD_WR_CMD(0x0050, 0x0000); // Horizontal GRAM Start Address
	LCD_WR_CMD(0x0051, 0x00EF); // Horizontal GRAM End Address
	LCD_WR_CMD(0x0052, 0x0000); // Vertical GRAM Start Address
	LCD_WR_CMD(0x0053, 0x013F); // Vertical GRAM Start Address
	LCD_WR_CMD(0x0060, 0xA700); // Gate Scan Line
	LCD_WR_CMD(0x0061, 0x0001); // NDL,VLE, REV
	LCD_WR_CMD(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
	LCD_WR_CMD(0x0080, 0x0000);
	LCD_WR_CMD(0x0081, 0x0000);
	LCD_WR_CMD(0x0082, 0x0000);
	LCD_WR_CMD(0x0083, 0x0000);
	LCD_WR_CMD(0x0084, 0x0000);
	LCD_WR_CMD(0x0085, 0x0000);
//-------------- Panel Control -------------------//
	LCD_WR_CMD(0x0090, 0x0010);
	LCD_WR_CMD(0x0092, 0x0000);
	LCD_WR_CMD(0x0093, 0x0003);
	LCD_WR_CMD(0x0095, 0x0110);
	LCD_WR_CMD(0x0097, 0x0000);
	LCD_WR_CMD(0x0098, 0x0000);
	LCD_WR_CMD(0x0007, 0x0133); // 262K color and display ON
  	
	//ini();

    LCD_WR_CMD(32, 0);
    LCD_WR_CMD(33, 0x013F);
	*(__IO uint16_t *) (Bank1_LCD_C)= 34;
	for(color1=0;color1<76800;color1++)
	{
	  LCD_WR_Data(0xffff);
	}
	color1=0;
	//while(1);								
}

//演示程序
void LCD_Test(unsigned int a)
{
	unsigned long n;

		n=0;
	
		LCD_WR_CMD(0x0050, 0); // Horizontal GRAM Start Address
		LCD_WR_CMD(0x0051, 239); // Horizontal GRAM End Address
		LCD_WR_CMD(0x0052, 0); // Vertical GRAM Start Address
		LCD_WR_CMD(0x0053, 319); // Vertical GRAM Start Address	 
		LCD_WR_CMD(32, 0);
    	LCD_WR_CMD(33, 0);
		LCD_WR_REG(34);
		while(n<153600)
		{
		     //while(1){

			//temp=(uint16_t)( a2[n]<<8)+a2[n+1];
			  //temp++;	
			  //LCD_WR_CMD(0x20, 0xa4);			
			LCD_WR_Data(a);
			n++;
			// }
			 //LCD_WR_Data(color1);
	 	}
//	delay(0xffff);

   
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

