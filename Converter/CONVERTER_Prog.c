
#include "StdTypes.h"
#include "LCD_Int.h"
#include "KEYPAD_Int.h"
#include "CONVERTER_Int.h"


#define  F_CPU   8000000
#include <util/delay.h>

static u8 Convert_to_Dec (u32 num)
{
	u8 dec_num=0, arr_Binary [8]={1,2,4,8,16,32,64,128};
	for(s8 i=0; i<8;i++)
	{
		if((num>>i)&1)
		{
			dec_num=dec_num+arr_Binary[i];
		}
	}
	return dec_num;
}

static u8 Select_Char (u8 lcd) // handle return
{
	u8 k,ind=10;
	u8 arr_Hex [16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	LCD_SetCursor(1,lcd);
	LCD_WriteChar(arr_Hex[ind]);	
	
	while(1)
	{
		k=KEYPAD_GetKey();
		if(k!=NO_KEY)
		{
			
			if(k=='A')
			{
				if(ind==15)
				ind=9;
					
				ind++;
				LCD_SetCursor(1,lcd);
				k=arr_Hex[ind];
				LCD_WriteChar(k);
			}
			else if(k!='A')
			{
				return ind;
			}
		}
		
		
	}
	
} 

static u8 Dec_to_Hex_Bin (u8 mode) // finished
{
	u8 num=0,k,flag_start=0x55,flag_count=0;
	static u8 flag_repeat=0x55;
	if(flag_repeat==0x55)
	{
		LCD_Clear();
		LCD_SetCursor(1,1);
		LCD_WriteString("DECIMAL:");
		LCD_SetCursor(2,1);
		
		if(mode=='1') // TO HEX
		{
			LCD_WriteString("HEX DEC:");
		}
		else if(mode=='2') // TO BINARY
		{
			LCD_WriteString("BINARY :");
		}
	} 
	else if(flag_repeat==0XAA)
	{
		LCD_SetCursor(1,9);
		LCD_WriteString("          ");
		LCD_SetCursor(2,9);
		LCD_WriteString("          ");
		flag_repeat=0x55;
	}
	
	while(flag_count!=3)
	{
		k=KEYPAD_GetKey();
		if(k=='c')
		{
			Converter_Init();
			k=0;
			return k;
		}
		if (k!=NO_KEY)
		{
			if(k=='=')
			{
				break;
			}
			if(k>='0'&&k<='9')
			{
				if (flag_start==0x55)
				{
					LCD_SetCursor(1,9);
					flag_start=0xAA;
				}
				num=num*10+k-'0';
				LCD_WriteChar(k);
				flag_count++;
			}
		}
	}
	
	if(mode=='1') // TO HEX
	{
		LCD_SetCursor(2,9);
		LCD_WriteHex(num);
	}
	else if(mode=='2') // TO BINARY
	{
		LCD_SetCursor(2,9);
		LCD_WriteBinary(num);
	}
	
	k=NO_KEY;
	while(k!='D'&&k!='c')
	{
		k=KEYPAD_GetKey();
		if (k=='D')
		{
			flag_repeat=0XAA;
		}
	}
	
	return k;
}

static u8 Bin_to_Dec_Hex (u8 mode) // finished
{
	u8 num=0,k,flag_start=0x55,flag_count=0;
	u8 dec_num=0;
	static u8 flag_repeat=0x55;
	
	
	if(flag_repeat==0x55)
	{
		LCD_Clear();
		LCD_SetCursor(1,1);
		LCD_WriteString("BINARY :");
		LCD_SetCursor(2,1);
		if(mode=='1') // TO HEX
		{
			LCD_WriteString("DECIMAL:");
		}
		else if(mode=='2') // TO BINARY
		{
			LCD_WriteString("HEX DEC:");
		}
	}
	else if(flag_repeat==0XAA)
	{
		LCD_SetCursor(1,9);
		LCD_WriteString("          ");
		LCD_SetCursor(2,9);
		LCD_WriteString("          ");
		flag_repeat=0x55;
	}
	while(flag_count!=8)
	{
		k=KEYPAD_GetKey();
		if(k=='c')
		{
			Converter_Init();
			k=0;
			return k;
		}
		if (k!=NO_KEY)
		{
			if(k=='0' || k=='1')
			{
				if (flag_start==0x55)
				{
					LCD_SetCursor(1,9);
					flag_start=0xAA;
				}
				LCD_WriteChar(k);
				k=k-'0';
				num=(num<<1)|k;
				flag_count++;
			}
		}
	}
	
	dec_num=Convert_to_Dec(num);
	
	if(mode=='1') // TO DEC
	{
		LCD_SetCursor(2,9);
		LCD_WriteNumber(dec_num);
	}
	else if(mode=='2') // TO HEX
	{
		LCD_SetCursor(2,9);
		LCD_WriteHex(dec_num);
	}
	
	k=NO_KEY;
	while(k!='D'&&k!='c')
	{
		k=KEYPAD_GetKey();
		if (k=='D')
		{
			flag_repeat=0XAA;
		}
	}
	
	return k;
}

static u8 Hex_to_Dec_Bin (u8 mode) // don't finish
{
	u8 num=0,k,ind,flag_start=0x55,flag_count=0,flag_select_ch=0x55;
	static u8 flag_repeat=0x55;
	u8 arr_Hex [16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	u8 _1st_digit=0,_2nd_digit=0;	
	if(flag_repeat==0x55)
	{
		LCD_Clear();
		LCD_SetCursor(1,1);
		LCD_WriteString("HEX DEC:");
		LCD_SetCursor(2,1);
		if(mode=='1')
		{
			LCD_WriteString("DECIMAL:");
		}
		else if(mode=='2')
		{
			LCD_WriteString("BINARY : ");
		}
		
	}	
	else if(flag_repeat==0XAA)
	{
		LCD_SetCursor(1,9);
		LCD_WriteString("          ");
		LCD_SetCursor(2,9);
		LCD_WriteString("          ");
		flag_repeat=0x55;
	}
	
	
	while(flag_count!=2)
	{
		k=KEYPAD_GetKey();
		if(k=='c')
		{
			Converter_Init();
			k=0;
			return k;
		}
		if (k!=NO_KEY)
		{
			for(ind=0;ind<11;ind++)
			{
				if(arr_Hex[ind]==k)
				{
					if (flag_start==0x55)
					{
						LCD_SetCursor(1,9);
						
						if(arr_Hex[ind]=='A')
						{
							ind=Select_Char(9); // position for print on lcd for 1st >> cursor(1,9)
							k=arr_Hex[ind];
							flag_select_ch=0XAA;
						}
						
						flag_start=0xAA;
						_1st_digit=ind;
					}
					else 
					{
						if(arr_Hex[ind]=='A')
						{
							ind=Select_Char(10); // position for print on lcd for 2nd >> cursor(1,10)
							k=arr_Hex[ind];
							flag_select_ch=0XAA;
						}
						_2nd_digit=ind;
					}
					
					if(flag_select_ch==0X55)
					{
						LCD_WriteChar(k);
					}
					flag_select_ch=0X55;
					flag_count++;
				}
			}
			
		}
	}
	
	num=(_1st_digit<<4)|_2nd_digit;
	
	if(mode=='1') // TO DEC
	{
		LCD_SetCursor(2,9);
		LCD_WriteNumber(num);
	}
	else if(mode=='2') // TO BINARY
	{
		LCD_SetCursor(2,9);
		LCD_WriteBinary(num);
	}
	
	k=NO_KEY;
	while(k!='D'&&k!='c')
	{
		k=KEYPAD_GetKey();
		if (k=='D')
		{
			flag_repeat=0XAA;
		}
	}
	
	return k;
}

void Converter_Init (void)
{
	static u8 flag=0x55;
	if(0x55==flag)
	{
		LCD_Clear();
		Welcome();
		flag=0XAA;
	}
	
	LCD_Clear();
	LCD_SetCursor(1,1);
	LCD_WriteString("Convert From : ");
	LCD_SetCursor(2,1);
	LCD_WriteString("1-D   2-H   3-B");
}


void Converter_Runnable (void) 
{
	static u8 sel1=0,sel2=0,k;
	static u8 flag=0x55;
	
	k=KEYPAD_GetKey();
	
	if(k!=NO_KEY)
	{
		if(k=='c')
		{
			sel1=0;
			sel2=0;
			flag=0x55;
			Converter_Init();
		}
		if(flag==0XAA)
		{
			if(k=='1'|| k=='2')
			{
				LCD_Clear();
				sel2=k;
				LCD_WriteChar(sel1);
				LCD_WriteChar(sel2);
				switch (sel1)
				{
					case '1':
					k=Dec_to_Hex_Bin(sel2);
					while(k=='D')
					{
						k=Dec_to_Hex_Bin(sel2);
					}
					if(k=='c')
					Converter_Init();
					
					sel1=0;
					sel2=0;
					flag=0x55;
					k=0;
					break;
					
					case '2':
					k=Hex_to_Dec_Bin(sel2);
					while(k=='D')
					{
						k=Hex_to_Dec_Bin(sel2);
					}
					if(k=='c')
					Converter_Init();
					
					sel1=0;
					sel2=0;
					flag=0x55;
					k=0;
					break;
					
					case '3':
					k=Bin_to_Dec_Hex(sel2);
					while(k=='D')
					{
						k=Bin_to_Dec_Hex(sel2);
					}
					if(k=='c')
					Converter_Init();
					
					sel1=0;
					sel2=0;
					flag=0x55;
					k=0;
					break;
					
				}
			}
			
		}
			
		if(flag==0X55)
		{
			
			if(k>='1'&&k<='3')
			{
				LCD_Clear();
				sel1=k;
				switch(sel1)
				{
					case '1' :
					LCD_SetCursor(1,1);
					LCD_WriteString("From D TO --");
					LCD_SetCursor(2,1);
					LCD_WriteString("1-HEX    2-BIN");
					break;
					case '2' :
					LCD_SetCursor(1,1);
					LCD_WriteString("From H TO --");
					LCD_SetCursor(2,1);
					LCD_WriteString("1-DEC    2-BIN");
					break;
					case '3' :
					LCD_SetCursor(1,1);
					LCD_WriteString("From B TO --");
					LCD_SetCursor(2,1);
					LCD_WriteString("1-DEC    2-HEX");
					break;
				}
				flag=0XAA;
			}
				
		}
	
	}
}
