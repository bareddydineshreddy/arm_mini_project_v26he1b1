#include<LPC21xx.h>
#include"funs.h"
#include"lcd.h"
#include"lcd_definesh.h"
#include"delay.h"
#include"kpm.h"

void eint0_fiq_isr(void) __irq;
void rtc_lcd(void);
unsigned int flag=0,ec;

int main(){
	
	unsigned char last;
	unsigned int num=0;
	
	InitLCD();
	init_eint();
	VICVectAddr0=(unsigned)eint0_fiq_isr;
	init_rtc();
	InitKPM();
	init_adc(0);

	HOUR=16;
	MIN=15;
	SEC=1;
	IODIR0|=(1<<7)|(1<<5)|(1<<0);
	
	while(1)
		{
			rtc_lcd();
			IOPIN0^=(1<<5);
			flag=edits(flag,&num,&last);
			led_status();
			
				
		}	
}


void eint0_fiq_isr(void) __irq
	{
	CmdLCD(CLEAR_LCD);
	StrLCD("Interupt Occured");
	delay_ms(500);
	IOPIN0^=1<<7;
	CmdLCD(CLEAR_LCD);
		flag=(flag)?0:1;
	//flag=1;
	EXTINT=1<<0;
	VICVectAddr=0;
}


