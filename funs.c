#include<LPC214x.h>
//#include<LPC21xx.h>
#include"lcd.h"
#include"lcd_definesh.h"
#include"kpm.h"
#include"delay.h"

void init_eint(void){
	PINSEL0&=~(3<<(1*2));
	PINSEL0|=(3<<2);
	VICIntSelect&=~(1<<14);
	VICVectCntl0=(1<<5)|14;
	//VICVectAddr0=(unsigned)eint0_fiq_isr;
	VICIntEnable=(1<<14);
	EXTINT=1<<0;
	EXTMODE=1<<0;
	EXTPOLAR=0;
}

void init_rtc(void){
	
	CCR=1<<1;
	CCR=1<<0;
	PREINT=456;
	PREFRAC=25024;
	//CCR=1<<4;

}
void init_adc(unsigned int ch){
	PINSEL1&=~(3<<((27-16)*2));
	PINSEL1|=(1<<((27-16)*2));
	AD0CR=0x00000000;
	AD0CR|=1<<ch;
	AD0CR|=13<<8;
	//AD0CR|=1<<16;
	AD0CR|=1<<21;
}

float adc(void){
	unsigned int res;
	float v;
	AD0CR|=1<<24;
	while(((AD0GDR>>31)&1)==0);
	res=((AD0GDR>>6)&0x3FF);
	v=res*(3.3/1023);
	CmdLCD(GOTO_LINE1_POS0+9);
	StrLCD("LCD:");
	CmdLCD(GOTO_LINE1_POS0+13);
	F32LCD(v,2);
	return v;
}

void rtc_lcd(){
	CmdLCD(GOTO_LINE1_POS0);
	CharLCD((HOUR/10)+48);
	CharLCD((HOUR%10)+48);
	CharLCD(':');
	CharLCD((MIN/10)+48);
	CharLCD((MIN%10)+48);
	CharLCD(':');
	CharLCD((SEC/10)+48);
	CharLCD((SEC%10)+48);


}

unsigned int edits(unsigned int flag , unsigned int *num ,unsigned char *last)
	{
	unsigned int temp,op;
	if(flag==1)	{
			CmdLCD(CLEAR_LCD);
			StrLCD("Edit1.HH2.MM3.SS");
			op=ReadNum();
			delay_ms(300);
				if(op==0)
					flag=0;
			//CmdLCD(GOTO_LINE1_POS0);
			CmdLCD(CLEAR_LCD);
			if(op==1){
				temp=HOUR;
				StrLCD("Enter hours ");
				ReadNum2(num,last);
				if(*num<=23)
				HOUR=*num;
				else
					{
						HOUR=temp;
					}
				*num=0;
				flag=0;
				CmdLCD(CLEAR_LCD);
			}
			else if(op==2){
				temp=MIN;
				StrLCD("Enter Minitus ");
				ReadNum2(num,last);
				if(*num<=59)
				MIN=*num;
				else
					{
						MIN=temp;
					}
				*num=0;
				flag=0;
				CmdLCD(CLEAR_LCD);
			}
			else if(op==3){
				temp=SEC;
				StrLCD("Enter Seconds ");
				ReadNum2(num,last);
				if(*num<=59)
				SEC=*num;
				else
					{
						SEC=temp;
					}
				num=0;
				flag=0;
				CmdLCD(CLEAR_LCD);
			}
		}
return flag;	
	}
	
void led_status(void)
	{	
		float adc_val=adc();
		if((HOUR>17) || (HOUR<7) || (adc_val<1.5)){
				IOCLR0=(1<<0);
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("LED:ON ");
				
			}
			else{
				IOSET0=(1<<0);
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("LED:OFF");
			}
	
	}
