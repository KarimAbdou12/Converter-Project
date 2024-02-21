/*
 * Converter.c
 *
 * Author : Karim Abdou
 */ 
/*=========================================*/

#include "StdTypes.h"
#include "MemMap.h"
#include "DIO_Int.h"
#include "LCD_Int.h"
#include "KEYPAD_Int.h"
#include "CONVERTER_Int.h"

#define  F_CPU   8000000
#include <util/delay.h>

/*=========================================*/



int main(void)
{
   DIO_Init();
   LCD_Init();
   KEYPAD_Init();
   Converter_Init();
    while (1) 
    {
		Converter_Runnable();
    }
}

