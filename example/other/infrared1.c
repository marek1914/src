#include <at89c51snd.h>

void InitCom(void);
void dispstr(unsigned char * pchar);
void disp(unsigned char);
void delay(void);
unsigned int i = 0;
unsigned int k = 0;
unsigned int j = 0;
unsigned int m = 0;
unsigned char xdata buffer[256]; //Default internal xdata
unsigned char xdata buffer1[257];// 
unsigned char xdata carry[200];
unsigned char xdata barry[200];
unsigned char xdata carry1;
unsigned char xdata carry2;

void int_0(void) interrupt 0
{
    TR1 = 1;	// Launch timer1
	TR0 = 1;    // Launch timer0  but controled by Gate^_^
    buffer[i++] = TH1;
	buffer[i++] = TL1;
	buffer1[k++] = TH0;
	buffer1[k++] = TL0;
	TH0 = 0;
	TL0 = 0;
	TH1 = 0;
	TL1 = 0;
    IE1 = 0;    // Clear Int Flag
	carry[j++] = carry1;
	barry[m++] = carry2;
	carry1 = 0;
	carry2 = 0;
}
void timer0(void) interrupt 1
{
   TF0 = 0;
   carry2++;
}

void timer1(void) interrupt 3
{
   TF1 = 0;
   carry1++;
}
void main()
{
    unsigned int gao;
    InitCom();
    EA = 1;        //  Enable Interrupt
	EX0 = 1;       //  Enable INT0 Interrupt
	ET0 = 1;
	ET1 = 1;
	CKCON |=0x06;  //  F_Timer0  = Focs/12  39ms total
   	IT0 = 1;       //  Edge-triggered
	TMOD = 0x19;   //  Timer1 :  mode1 16bit Timer   Timer0: mode1 Gate^_^
	TH0 = 0;
	TL0 = 0;
	TH1 = 0;
	TL1 = 0; 
	for (gao=0;gao<256;gao++)
	  buffer[gao] = 0;
	dispstr("Preparation....\n\15");
    while(1)
	  if(i>255) break;
	for (i=0,j=0;i<255;){
	    dispstr("0x");
		disp(carry[j++]);
	    disp(buffer[i++]);
		disp(buffer[i++]);
		dispstr(" ");
	}

    for (i=0,j=0;i<255;){
	    dispstr("0x");
		disp(barry[j++]);
	    disp(buffer1[i++]);
		disp(buffer1[i++]);
		dispstr(" ");
	}

	while(1);
}

