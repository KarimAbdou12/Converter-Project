/*
 * KEYPAD_Cfg.h
 *
 * Created: 2/10/2024 7:26:28 AM
 *  Author: Magic
 */ 


#ifndef KEYPAD_CFG_H_
#define KEYPAD_CFG_H_


#define  ROW   4
#define  COL   4

#define  FIRST_OUTPUT  PINB4
#define  FIRST_INPUT   PIND2

#define  NO_KEY   't'

#ifdef KEYPAD_PROG

u8 KeysArray[ROW][COL]={{'7','8','9','A'},
{'4','5','6','B'},
{'1','2','3','C'},
{'c','0','=','D'}
};

#endif



#endif /* KEYPAD_CFG_H_ */