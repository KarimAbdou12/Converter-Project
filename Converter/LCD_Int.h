


#ifndef LCD_INT_H_
#define LCD_INT_H_


void LCD_Init(void);
void WriteData(u8 data);
void LCD_WriteChar(u8 ch);
void LCD_WriteNumber(s32 num);
void LCD_WriteNumber4D (u16 num);
void LCD_WriteBinary(u8 num);
void LCD_WriteHex(u8 num);
void LCD_CustomChar(u8 location,u8*pattern);
void LCD_FUNC (void);
void _1st_line (void);
u8 escape (u8 arrow);
void Welcome (void);

void LCD_WriteString(char*str);
void LCD_SetCursor(u8 line,u8 cell);
void LCD_Clear(void);



#endif /* LCD_INT_H_ */