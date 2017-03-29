#include <reg51.h>

unsigned char cod[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void InitCom(void)
{
    CKCON = CKCON|0x01;
    SCON  = 0x50;
    BDRCON = 0x1e;
    PCON=PCON|0x80;
    BRL = 126;
} 

void disp(unsigned char * pchar)
{
    while (*pchar) {
        SBUF = *pchar++;
        while (!TI);
        TI = 0;
    }
} 

void disp(unsigned char number)
{
    SBUF = cod[(number&0xf0)>>4]; 
    while (!TI);
    TI = 0;
    SBUF = cod[number&0x0f];
    while (!TI);
    TI = 0;
}

void main(void)
{
    InitCom();
    disp("hello\n");
   
}

