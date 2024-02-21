
#include "StdTypes.h"
#include "DIO_Int.h"
#include "LCD_Cfg.h"
#include "LCD_Int.h"

#define F_CPU 8000000
#include <util/delay.h>

#if LCD_Mode==_8_BIT
void WriteIns(u8 ins)
{
	DIO_WritePin(RS,LOW);
	DIO_WritePort(LCD_PORT,ins);
	// pulse
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	


void WriteData(unsigned char data)
{
	DIO_WritePin(RS,HIGH);
	DIO_WritePort(LCD_PORT,data);
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
}

void LCD_Init(void)
{
	_delay_ms(50);
	
	WriteIns(0x38);// func set
	WriteIns(0x0C);// display on/off 
	WriteIns(0x01);// clear that make us written on lcd ( DDRAM ) adress 0x00 (from data sheet)
	_delay_ms(1); // delay need 2 msec
	WriteIns(0x06);// entry (increase DDRAM address)
}

#elif LCD_Mode==_4_BIT
void WriteIns(u8 ins)
{
	
	DIO_WritePin(RS,LOW);
	
	DIO_WritePin(D7,READ_BIT(ins,7));
	DIO_WritePin(D6,READ_BIT(ins,6));
	DIO_WritePin(D5,READ_BIT(ins,5));
	DIO_WritePin(D4,READ_BIT(ins,4));
	// pulse
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	
	DIO_WritePin(D7,READ_BIT(ins,3));
	DIO_WritePin(D6,READ_BIT(ins,2));
	DIO_WritePin(D5,READ_BIT(ins,1));
	DIO_WritePin(D4,READ_BIT(ins,0));
	// pulse
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	
}

void WriteData(unsigned char data)
{
	DIO_WritePin(RS,HIGH); 
	
	DIO_WritePin(D7,READ_BIT(data,7));
	DIO_WritePin(D6,READ_BIT(data,6));
	DIO_WritePin(D5,READ_BIT(data,5));
	DIO_WritePin(D4,READ_BIT(data,4));

	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	
	DIO_WritePin(D7,READ_BIT(data,3));
	DIO_WritePin(D6,READ_BIT(data,2));
	DIO_WritePin(D5,READ_BIT(data,1));
	DIO_WritePin(D4,READ_BIT(data,0));
	// pulse
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	// more than in data sheet that meaning pulse
}

void LCD_Init(void)
{
	_delay_ms(50);
	
	WriteIns(0x02);
	WriteIns(0x28);
	
	
	WriteIns(0x0C);// display on/off >> try ( 0x0e   0x0f )
	WriteIns(0x01);// clear that make us written on lcd ( DDRAM ) adress 0x00 (from data sheet)
	_delay_ms(1); // delay need 2 msec
	WriteIns(0x06);// entry (increase DDRAM address)
}
#endif

void LCD_WriteChar(u8 ch)
{
	WriteData(ch);
}

void LCD_WriteNumber(s32 num)
{
	u8 str[10],i=0;
	s8 j;
	if(num==0)
	{
		LCD_WriteChar('0');
	}
	else
	{
		if(num<0)
		{
			num=num*-1;
			LCD_WriteChar('-');
		}
		while(num)
		{
			str[i]=num%10+'0';
			i++;
			num=num/10;
		}
		for(j=i-1;j>=0;j--)
		{
			LCD_WriteChar(str[j]);
		}
	}
}

void LCD_WriteNumber4D (u16 num)
{
	LCD_WriteChar(((num%10000)/1000)+'0');
	LCD_WriteChar(((num%1000)/100)+'0');
	LCD_WriteChar(((num%100)/10)+'0');
	LCD_WriteChar(((num%10)/1)+'0');
}

void LCD_WriteString(char*str)
{
	u8 i;
	for (i=0;str[i];i++)
	{
		WriteData(str[i]);
		_delay_ms(40);
	}
}

void LCD_SetCursor(u8 line,u8 cell)
{
	if (line==1)
	{
		WriteIns(0x80|(cell-1));
	}
	else if (line ==2)
	{
		WriteIns(0x80|0x40|(cell-1));
	}
}

void LCD_Clear(void)
{
	WriteIns(0x01);
	_delay_ms(1);
}

void LCD_WriteBinary(u8 num)
{
	s8 i;
	for(i=7;i>=0;i--)
	{
		LCD_WriteChar(READ_BIT(num,i)+'0');
	}
}

void LCD_WriteHex(u8 num)
{
	u8 HN=num>>4;
	u8 LN=num&0x0f;
	if(HN<10)
	{
		LCD_WriteChar(HN+'0');
	}
	else
	{
		LCD_WriteChar(HN-10+'A');
	}
	if(LN<10)
	{
		LCD_WriteChar(LN+'0');
	}
	else
	{
		LCD_WriteChar(LN-10+'A');
	}
	
}


void LCD_CustomChar(u8 location,u8*pattern)
{
	WriteIns(0x40+location*8);  
	s8 i;
	for(i=0;i<8;i++)
	{
		WriteData(pattern[i]);
	}
	LCD_SetCursor(1,1); 
	
	 
}


void LCD_FUNC (void)
{
	WriteIns(0X40+0);
	WriteData(0X55);
	WriteData(0XAA);
	WriteData(0X55);
	WriteData(0XAA);
	WriteData(0X55);
	WriteData(0XAA);
	WriteData(0X55);
	
}

void _1st_line (void)
{
	s8 i=1;
	for(i=1;i<=12;i++)
	{
		LCD_SetCursor(1,i);
		LCD_WriteString("karim");
		_delay_ms(300);
		LCD_Clear();
	}
}

u8 escape (u8 arrow)
{
	while(DIO_ReadPin(PIND2)==0)
	{
		LCD_Clear();
		LCD_SetCursor(2,1);
		LCD_WriteChar(0);
		LCD_SetCursor(1,14);
		LCD_WriteChar(1);
		LCD_SetCursor(2,2);
		LCD_WriteChar(2);
		LCD_SetCursor(2,arrow);
		LCD_WriteChar(3);
		arrow++;
		_delay_ms(200);
	}
	return arrow;
}

void Welcome (void)
{

	LCD_SetCursor(1,5);
	LCD_WriteString("WELCOME");
	LCD_SetCursor(2,2);
	LCD_WriteString("TO CONVERTER");
	_delay_ms(1000);
	LCD_Clear();
	
	LCD_SetCursor(1,6);
	LCD_WriteString("START");
	_delay_ms(200);
	LCD_Clear();
	
}