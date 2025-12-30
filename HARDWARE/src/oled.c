#include "oled.h"
#include "pic.h"
//#include "default.h"

/**
  * @brief  OLED管脚初始化
  * @param  void
  * @retval void
	* @note 
	* PB6   -- OLED_RES	 复位，低电平有效
	* PB7   -- OLED_CS	   
	* PB12  -- OLED_DC 	 选择发送指令/数据
	* PB13  -- OLED_SCK
	* PB15  -- OLED_MOSI
  */
void OLED_Pin_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//PB6 PB7 P12
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	OLED_CS_H;
	OLED_SCK_H;
	OLED_UNRESET;
}

/**
  * @brief  SPI1传输数据
  * @param  void
  * @retval void
	* @note 
  */
void OLED_Transferdata(u8 data)
{
	for(u8 i=0;i<8;i++)
	{
		OLED_SCK_L;//拉低时钟线	
		if(data&(0x80)>>i)
			OLED_MOSI_H;
		else 
			OLED_MOSI_L;
		
		OLED_SCK_H;
	}		
}

/**
  * @brief  OLED发送命令
  * @param  u8 parametr
  * @retval void
	* @note 
  */
void OLED_Send_Cmd(u8 parametr)
{
	OLED_CMD;//选择发送命令
	OLED_CS_L;
	OLED_Transferdata(parametr);
	OLED_CS_H;
}


/**
  * @brief  OLED发送数据
  * @param  u8 parametr
  * @retval void
	* @note 
  */
void OLED_Send_Data(u8 parametr)
{
	OLED_DATA;//选择发送数据
	OLED_CS_L;
	OLED_Transferdata(parametr);
	OLED_CS_H;
}




/**
  * @brief  清屏函数（往指定位置填充颜色）
  * @param  u8 color  0黑色  1白色
  * @retval void
	* @note 
  */
void OLED_Clear(u8 color)
{
	for(u8 i=0;i<8;i++)
	{
		OLED_Send_Cmd(0xB0 + i);
		OLED_Send_Cmd(0x00);  // 低4位列地址（0x00 ~ 0x0F）
		OLED_Send_Cmd(0x10);  // 高4位列地址起始（0x10 ~ 0x17）
		for(u8 j=0;j<128;j++)
		{
			OLED_Send_Data(color);
		}
	}
}

void OLED_SetClear(u8 col,u8 page,u8 width,u8 high,u8 color)
{
	for(u8 i=page;i<page + high;i++)
	{
		OLED_Send_Cmd(0xB0 + i);//页起始位
		OLED_Send_Cmd(0x00 + (col&0x0F));       // 低4位列地址（0x00 ~ 0x0F）
		OLED_Send_Cmd(0x10 + ((col&0xF0)>>4));  // 高4位列地址起始（0x10 ~ 0x17）
		for(u8 j=col;j<col+width;j++)//列
		{
			OLED_Send_Data(color);
		}
	}
}

/**
  * @brief  OLED初始化
  * @param  void
  * @retval void
	* @note 
  */
void OLED_Init(void)
{
	//管脚初始化
	OLED_Pin_Init();
	
	//上电时间（复位/延时）
	OLED_UNRESET;
	delay_ms(100);
	OLED_RESET;
	delay_ms(200);
	OLED_UNRESET;
	delay_ms(100);
	
	/* start initial sequence */ 
	OLED_Send_Cmd(0xAE); //关闭显示
  OLED_Send_Cmd(0xD5); //设置时钟分频因子,震荡频率
  OLED_Send_Cmd(0x80);   //[3:0],分频因子;[7:4],震荡频率
  OLED_Send_Cmd(0xA8); //设置驱动路数
  OLED_Send_Cmd(0X3F); //默认0X3F(1/64)
  OLED_Send_Cmd(0xD3); //设置显示偏移
  OLED_Send_Cmd(0X00); //默认为0

  OLED_Send_Cmd(0x40); //设置显示开始行 [5:0],行数.

  OLED_Send_Cmd(0x8D); //电荷泵设置
  OLED_Send_Cmd(0x14); //bit2，开启/关闭
  OLED_Send_Cmd(0x20); //设置内存地址模式
  OLED_Send_Cmd(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
  OLED_Send_Cmd(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
  OLED_Send_Cmd(0xC8); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
  OLED_Send_Cmd(0xDA); //设置COM硬件引脚配置
  OLED_Send_Cmd(0x12); //[5:4]配置

  OLED_Send_Cmd(0x81); //对比度设置
  OLED_Send_Cmd(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
  OLED_Send_Cmd(0xD9); //设置预充电周期
  OLED_Send_Cmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
  OLED_Send_Cmd(0xDB); //设置VCOMH 电压倍率
  OLED_Send_Cmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  OLED_Send_Cmd(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
  OLED_Send_Cmd(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
  OLED_Send_Cmd(0xAF); //开启显示
		
	//清屏函数
	OLED_Clear(0x00);
}


/**
  * @brief  显示图片
  * @param  
						u8 col   图片起始列
						u8 page	 图片起始页
						u8 width 图片宽度
						u8 high  图片高度				
						u8 *pic  指向图片数据
  * @retval void
	* @note 
  */
void OLED_DisplayPic(u8 col,u8 page,u8 width,u8 high,u8*pic)
{
	//几页
	for(u8 i=page;i<page + high;i++)
	{
		OLED_Send_Cmd(0xB0 + i);//页起始位
		OLED_Send_Cmd(0x00 + (col&0x0F));       // 低4位列地址（0x00 ~ 0x0F）
		OLED_Send_Cmd(0x10 + ((col&0xF0)>>4));  // 高4位列地址起始（0x10 ~ 0x17）
		for(u8 j=col;j<col+width;j++)//列
		{
			OLED_Send_Data(*pic++);
		}
	}
}

/**
  * @brief  显示图片
  * @param  
						u8 col   图片起始列
						u8 page	 图片起始页
						u8 width 图片宽度
						u8 high  图片高度				
						u8 *pic  指向图片数据
  * @retval void
	* @note 
  */
void OLED_DisplayNewPic(u8 col,u8 page,u8 width,u8 high,u8 front,u8*pic)
{
	//几页
	for(u8 i=page;i<page + high;i++)
	{
		OLED_Send_Cmd(0xB0 + i);//页起始位
		OLED_Send_Cmd(0x00 + (col&0x0F));       // 低4位列地址（0x00 ~ 0x0F）
		OLED_Send_Cmd(0x10 + ((col&0xF0)>>4));  // 高4位列地址起始（0x10 ~ 0x17）
		pic += front;////跳过超出屏幕的数据在换行，前面
		for(u8 j=col;j<col+width;j++)//列
		{
			OLED_Send_Data(*pic++);
			if(j>=127)
			{
				pic += width - (128-col);//跳过超出屏幕的数据在换行，用于后面
				break;
			}
		}
	}
}

/**
  * @brief 显示字符
  * @param  
						u8 col 	列起始
						u8 page	页起始
						u8 *pic 字模数据
  * @retval void
	* @note 
  */
void OLED_DisplayChar(u8 col,u8 page,u8 pic)
{
	u8 i,j;
	u16 num = (pic-' ')*16;
	for(i=page;i<page+2;i++)//字符需要占两页
	{
		OLED_Send_Cmd(0xB0 + i);//设置页地址
		OLED_Send_Cmd(0x00 + (col&0x0F));  // 低4位列地址（0x00 ~ 0x0F）
		OLED_Send_Cmd(0x10 + ((col&0xF0)>>4));  // 高4位列地址起始（0x10 ~ 0x17）
		for(j=0;j<8;j++)//字符宽度
		{
			OLED_Send_Data(Aciss_8X16[num++]);
		}
	}
}


/**
  * @brief 显示汉字
  * @param  
						u8 col   字体起始列
						u8 page	 字体起始页
						u8 size  字体大小
						u8 *pic  字模数据
  * @retval void
	* @note 
  */
void OLED_DisplayChin(u8 col,u8 page,u8*pic)
{
	for(u8 i=page;i<page+2;i++)//页
	{
		OLED_Send_Cmd(0xB0 + i);
		OLED_Send_Cmd(0x00 + (col&0x0F));  // 低4位列地址（0x00 ~ 0x0F）
		OLED_Send_Cmd(0x10 + ((col&0xF0)>>4));  // 高4位列地址起始（0x10 ~ 0x17）
		for(u8 j=0;j<16;j++)
		{
			OLED_Send_Data(*pic++);
		}
	}
}


/**
  * @brief 显示任意字符
  * @param  
						u8 col   字体起始列
						u8 page	 字体起始页
						u8 size  字体大小
						u8 *pic  字模数据
  * @retval void
	* @note 
  */
void OLED_DisplayStr(u8 col,u8 page,u8*pic)
{
	while(*pic != '\0')
	{
		if(*pic>0xA1)//中文
		{
			for(u8 n=0;n<sizeof(HZ16)/sizeof(typFNT_GB16);n++)//找遍的字库
			{
				//找到了
				if((*pic == HZ16[n].Index[0]) && (*(pic+1) == HZ16[n].Index[1]))
				{					
					OLED_DisplayChin(col,page,(u8*)HZ16[n].Msk);
					col+=16;
					pic+=2;
					break;
				}
			}
		}
		else//英文
		{
			OLED_DisplayChar(col,page,*pic);
			col+=8;
			pic+=1;
		}
	}
}


