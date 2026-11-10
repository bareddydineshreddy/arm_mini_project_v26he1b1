void delay_us(unsigned int dlyUS)
{
	for(dlyUS*=12;dlyUS>0;dlyUS--);
}
void delay_ms(unsigned int dlyMS)
{
for(dlyMS*=12000;dlyMS>0;dlyMS--);
}
void delay_s(unsigned int dlyS)
{
	for(dlyS*=12000000;dlyS>0;dlyS--);
}
