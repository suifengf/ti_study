#include "lcd_init.h"
// #include "delay.h" // 确保你的 delay 包含进来

void LCD_GPIO_Init(void)
{
    // GPIO初始化应在外部SysConfig或此处完成
}

/******************************************************************************
      函数说明：LCD串行数据写入函数 (保持原样)
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
    LCD_CS_Clr();

    //发送数据
    DL_SPI_transmitData8(SPI_LCD_INST, dat);
    //等待SPI总线空闲
    while(DL_SPI_isBusy(SPI_LCD_INST));

    LCD_CS_Set();
}

/******************************************************************************
      基础写入函数 (保持原样)
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}

void LCD_WR_DATA(u16 dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}

void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//写数据
}

/******************************************************************************
      函数说明：设置起始和结束地址
      修改点 2：1.8寸屏的显存通常存在偏移，竖屏横坐标偏移+2，纵坐标偏移+1。
               横屏则反过来。参照了逐飞库中的 `tft180_set_region`。
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0 || USE_HORIZONTAL==1) // 竖屏
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+2);
		LCD_WR_DATA(x2+2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+1);
		LCD_WR_DATA(y2+1);
		LCD_WR_REG(0x2c);//储存器写
	}
	else // 横屏 (USE_HORIZONTAL==2 || USE_HORIZONTAL==3)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+1);
		LCD_WR_DATA(x2+1);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+2);
		LCD_WR_DATA(y2+2);
		LCD_WR_REG(0x2c);//储存器写
	}
}

/******************************************************************************
      函数说明：LCD初始化
      修改点 3：替换为逐飞 STC32G 开源库中 1.8寸屏幕的寄存器初始化序列。
******************************************************************************/
void LCD_Init(void)
{
	LCD_GPIO_Init();//初始化GPIO
	
	LCD_RES_Clr();//复位
	delay_ms(10); // 根据逐飞库，低电平10ms即可
	LCD_RES_Set();
	delay_ms(120);
	
	LCD_BLK_Set();//打开背光
    delay_ms(10);
	
	//************* Start Initial Sequence 1.8 inch **********//
	LCD_WR_REG(0x11); // Sleep out 
	delay_ms(120);    // Delay 120ms 
	
	LCD_WR_REG(0xB1); 
	LCD_WR_DATA8(0x01); LCD_WR_DATA8(0x2C); LCD_WR_DATA8(0x2D); 

	LCD_WR_REG(0xB2); 
	LCD_WR_DATA8(0x01); LCD_WR_DATA8(0x2C); LCD_WR_DATA8(0x2D); 

	LCD_WR_REG(0xB3); 
	LCD_WR_DATA8(0x01); LCD_WR_DATA8(0x2C); LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x01); LCD_WR_DATA8(0x2C); LCD_WR_DATA8(0x2D); 

	LCD_WR_REG(0xB4); 
	LCD_WR_DATA8(0x07); 

	LCD_WR_REG(0xC0); 
	LCD_WR_DATA8(0xA2); LCD_WR_DATA8(0x02); LCD_WR_DATA8(0x84); 
    
	LCD_WR_REG(0xC1); 
	LCD_WR_DATA8(0xC5); 

	LCD_WR_REG(0xC2); 
	LCD_WR_DATA8(0x0A); LCD_WR_DATA8(0x00); 

	LCD_WR_REG(0xC3); 
	LCD_WR_DATA8(0x8A); LCD_WR_DATA8(0x2A); 
    
	LCD_WR_REG(0xC4); 
	LCD_WR_DATA8(0x8A); LCD_WR_DATA8(0xEE); 

	LCD_WR_REG(0xC5); 
	LCD_WR_DATA8(0x0E); 

	LCD_WR_REG(0x36); // 显示方向设置
	if(USE_HORIZONTAL==0)      LCD_WR_DATA8(0xC0); // 竖屏
	else if(USE_HORIZONTAL==1) LCD_WR_DATA8(0x00); // 竖屏旋转180
	else if(USE_HORIZONTAL==2) LCD_WR_DATA8(0xA0); // 横屏
	else                       LCD_WR_DATA8(0x60); // 横屏旋转180

	LCD_WR_REG(0xE0); 
	LCD_WR_DATA8(0x0F); LCD_WR_DATA8(0x1A); LCD_WR_DATA8(0x0F); LCD_WR_DATA8(0x18); 
	LCD_WR_DATA8(0x2F); LCD_WR_DATA8(0x28); LCD_WR_DATA8(0x20); LCD_WR_DATA8(0x22); 
	LCD_WR_DATA8(0x1F); LCD_WR_DATA8(0x1B); LCD_WR_DATA8(0x23); LCD_WR_DATA8(0x37); 
	LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x07); LCD_WR_DATA8(0x02); LCD_WR_DATA8(0x10); 

	LCD_WR_REG(0xE1); 
	LCD_WR_DATA8(0x0F); LCD_WR_DATA8(0x1B); LCD_WR_DATA8(0x0F); LCD_WR_DATA8(0x17); 
	LCD_WR_DATA8(0x33); LCD_WR_DATA8(0x2C); LCD_WR_DATA8(0x29); LCD_WR_DATA8(0x2E); 
	LCD_WR_DATA8(0x30); LCD_WR_DATA8(0x30); LCD_WR_DATA8(0x39); LCD_WR_DATA8(0x3F); 
	LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x07); LCD_WR_DATA8(0x03); LCD_WR_DATA8(0x10); 

	// 限制窗口寄存器初始值
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x82); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0x00); LCD_WR_DATA8(0x83); 

	LCD_WR_REG(0xF0); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_REG(0xF6); 
	LCD_WR_DATA8(0x00); 

	LCD_WR_REG(0x3A); 
	LCD_WR_DATA8(0x05); 
	LCD_WR_REG(0x29); // Display On
}