#include <at89c51snd.h>

void InitCom(void);
void dispstr(unsigned char * pchar);
void disp(unsigned char);
unsigned char intcounter = 0;
unsigned char timecounter = 0;
unsigned char buffer = 0;
void int_0(void) interrupt 0 using 0
{
	if(intcounter == 6){
		TH1 = 0xf2;
		TL1 = 0x44;	// 1.25 bit   1688 + 844/2
		TR1 = 1;
		EX0 = 0;    // Disable INT0 interrupt
		intcounter = 0; //counter clear to 0
	}
	intcounter++;	
	IE0 = 0;   	   // Clear Int Flag	
}

void timer1(void) interrupt 3 using 0
{
	TH1 = 0xf5;
	TL1 = 0x03;       // 844us 
	INT0 = 1; 
	if(!INT0) buffer |= 0x01;
	buffer <<= 1;
	timecounter++;
	TF1 = 0;
}

void main()
{
   unsigned char index;                                
	unsigned char code codetable0[] = {0x66,0x64,0x7c,0x7a,0x78,0x76,0x74,0x72,0x70,
	                                  0x6e,0x6c,0x7e,0x6a,0x42,0x3e,0x3c,0x28,0x26,
												 0x5e,0x5c,0x2c,0x62,0x60,0x32,0x46,0x44,0x56,
												 0x54,0x5a,0x58,0x0e,0x00};
	unsigned char* code codetable1[] = {"Turn on/off","volume off","1","2","3","4",
	               "5","6","7","8","9","0","-/--",">>>","PROG+","PROG-",
						"TUNE+","TUNE-","VOL+","VOL-","STORE","P.P","CALL","Time",
						"CONTRAST+","CONTRAST-","COLOR+","COLOR-","LIGHT+","LIGHT-",
						"AV","TV"};
 
   InitCom();
   EA = 1;       //  Enable Interrupt
	EX0 = 1;
	ET1 = 1;
	IT0 = 1;
	CKCON |=0x06;  //  F_Timer0  = Focs/12  39ms total
	TMOD = 0x11;   //  Timer1 :  mode1 16bit Timer  
	TH1 = 0;
	TL1 = 0; 
	buffer = 0;
	while(1){
		if(timecounter==6){
		   TR1 = 0;
			timecounter = 0;
			intcounter = 0;
			index = 0;
			for(;;){
				if(buffer==codetable0[index])break;
				index++;
			}
			dispstr("\n\15");
			dispstr(codetable1[index]);
			buffer = 0;
			IE0 = 0;
			EX0= 1;
		}	
	}
	
}
