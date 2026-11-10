//kpm2.c
#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "kpm_defines.h"
#include "lcd.h"
#include "lcd_definesh.h"

/*
u8 kpmLUT[4][4]=
{
	{0,1,2,3},
	{4,5,6,7},
	{8,9,10,11},
	{12,13,14,15}
};
*/
u8 kpmLUT[4][4]=
{
	{'1','2','3','a'},
	{'4','5','6','b'},
	{'7','8','9','c'},
	{'*','0','#','d'}
};

void InitKPM(void)
{
  //ground all row lines
	//IODIR1&=((1<<ROW0)|(1<<ROW1)|(1<<ROW2)|(1<<ROW3));
  SETBIT(IODIR1,ROW0);
	SETBIT(IODIR1,ROW1);
	SETBIT(IODIR1,ROW2);
	SETBIT(IODIR1,ROW3);
	//all col lines are input high
	//default any input & high
}	



u8 ColScan(void)
{
	u8 t;
	t=(READBIT(IOPIN1,COL0)&READBIT(IOPIN1,COL1)&READBIT(IOPIN1,COL2)&READBIT(IOPIN1,COL3));
	return t; 
}

u8 RowCheck(void)
{
	u8 r;
	
	// Grounding 0th row only
	IOCLR1 = (1 << ROW0);
	IOSET1 = (1 << ROW1) | (1 << ROW2) | (1 << ROW3);
	if(ColScan() == 0) { r = 0; goto last; }
	
	// Grounding 1st row only
	IOSET1 = (1 << ROW0);
	IOCLR1 = (1 << ROW1);
	IOSET1 = (1 << ROW2) | (1 << ROW3);
	if(ColScan() == 0) { r = 1; goto last; }
	
	// Grounding 2nd row only
	IOSET1 = (1 << ROW0) | (1 << ROW1);
	IOCLR1 = (1 << ROW2);
	IOSET1 = (1 << ROW3);
	if(ColScan() == 0) { r = 2; goto last; }
	
	// Grounding 3rd row only
	IOSET1 = (1 << ROW0) | (1 << ROW1) | (1 << ROW2);
	IOCLR1 = (1 << ROW3);
	if(ColScan() == 0) { r = 3; }

last:
	// Reset all rows low or high as needed
	IOCLR1 = (1 << ROW0) | (1 << ROW1) | (1 << ROW2) | (1 << ROW3);
	return r; 	
}

u8 ColCheck(void)
{
	u8 c;
	
	if(READBIT(IOPIN1,COL0)==0)
		c=0;
	else if(READBIT(IOPIN1,COL1)==0)
		c=1;
	else if(READBIT(IOPIN1,COL2)==0)
		c=2;
	else if(READBIT(IOPIN1,COL3)==0)
		c=3;
	
	return c;
}

u8 KeyScan(void)
{
	 u8 r,c,keyV;	 
   //wait for any key press
   while(ColScan());
   //if any key pressed,
   //identify row in which key was pressed
   r=RowCheck();
   //identify col in which key was pressed
   c=ColCheck();
   //extract key value from LUT
   keyV=kpmLUT[r][c];
	
   return keyV;	
}
u32 ReadNum(void)
{
	
	u32 keyV,sum=0;	
	while(1)
	{
		keyV=KeyScan();
		if((keyV>='0') && (keyV<='9'))
		{
			sum=(sum*10)+(keyV-'0');
			CmdLCD(GOTO_LINE2_POS0);
			U32LCD(sum);
			while(ColScan()==0);
			if (sum == 'd')
				return 0;
			return sum;
		}
		else
		{
			if(keyV=='C')
			{
				CmdLCD(CLEAR_LCD);
			}
			while(ColScan()==0);
			break;
		}
	}
	return sum;
}

void ReadNum2(u32 *num,u8 *lastKey)
{
	u32 count=0;
	while(1)
	{
		if(count>1)
			break;
		*lastKey=KeyScan();
		if((*lastKey>='0')&&(*lastKey<='9'))
		{
			count++;
			*num=(*num * 10)+(*lastKey-'0');
			CmdLCD(GOTO_LINE2_POS0);
			U32LCD(*num);
			while(ColScan()==0);
		}
		else
		{
			if(*lastKey=='C')
			{
				CmdLCD(CLEAR_LCD);
			}
			while(ColScan()==0);
			break;
		}
	}
}
