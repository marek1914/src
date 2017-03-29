void delay1ms(){		
} 

BOOL  ds2411_reset ( void )
{
	SetBit( pio_control_bit,HIGH) ;//ds2411_pio_set();
	SetBit( pio_control_bit,LOW) ;//ds2411_pio_clear();     
	while(count<560)
		{
		delay1us(); /*560us*/
		count++;
		}
	#if 1
	SetBit( pio_control_bit,HIGH) ;//ds2411_pio_set();    
	count = 0;
	while(count<70)
		{
		delay1us();//ds2411_delay(1400); 	/*70us*/
		count++;
		}
	#endif

	GetBit(pio_control_bit,&flag0);    

	count = 0;
	while(count<480)
		{
		delay1us(); /*480us*/
		count++;
		}
	
	return flag0;                                    
}

void ds2411_writebit(int bit)
{
	UI16 count = 0;
	if (bit)
	{
		SetBit( pio_control_bit,HIGH) ;//ds2411_pio_set();                       
		SetBit( pio_control_bit,LOW) ;//ds2411_pio_clear();  

		while(count<10)
			{
			delay1us();//ds2411_delay(180);    /*10.8us*/
			count++;
			}

		count = 0;
		SetBit( pio_control_bit,HIGH) ;//ds2411_pio_set(); 

		while(count<94)
			{
			delay1us();//ds2411_delay(1900);    /*94.2*/ 
			count++;
			}
	}
	else
	{
		SetBit( pio_control_bit,HIGH) ;//ds2411_pio_set();                          
		SetBit( pio_control_bit,LOW) ;//ds2411_pio_clear();    

		count = 0;
		while(count<90)
			{
			delay1us();//ds2411_delay(1800);	/*90.2us*/
			count++;
			}
		
		SetBit( pio_control_bit,HIGH) ;//ds2411_pio_set(); 

		count = 0;
		while(count<15)
			{
			delay1us();//ds2411_delay(280);  /*15us*/
			count++;
			}
	}
}


int ds2411_readbit(void)
{
    SetBit( pio_control_bit,HIGH) ;//ds2411_pio_set();                          
    SetBit( pio_control_bit,LOW) ;//ds2411_pio_clear();  

	while(count<10)
		{
		delay1us();//ds2411_delay(180);           /*10.8us*/ ]
		count++;
		}
#if 1	
	SetBit( pio_control_bit,HIGH) ;//ds2411_pio_set();  

	count = 0;
	while(count<5)
		{
		delay1us();//ds2411_delay(180);         /*10.2us*/  ]
		count++;
		}
#endif
    GetBit(pio_control_bit,&flag0);  

	count = 0;
	while(count<80)
		{
		delay1us();//ds2411_delay(1500);       /*80us*/ 
		count++;
		}

	return (flag0);
}


void ds2411_writebyte(int data)
{
	int i = 0;

	for (i=0; i<8; i++)
	{
		ds2411_writebit(data&0x01);

		data>>=1;                              
	}
}


int  ds2411_readbyte(void)
{
	int i,result = 0;

	for (i=0; i<8; i++)
	{
		if (ds2411_readbit())
			result|=0x01<<i;
	}
	return result;
}


void ds2411_delay(int us)        /*  0.05*us  */
{
	int i = 0;

	for (i=0; i<us; i++);
}


unsigned char  ds2411_crc8( unsigned char *ptr, unsigned char len)
{
	unsigned int i=0, j=0;
	unsigned char crc=0, crcbit[8];

	for (i=0; i<len; i++)
	{
		for (j=0x01; j<=0x80; (j=j<<1))
		{
			if ((crc&0x80)!=0)
			{
				crc=crc<<1;
				if (((*ptr)&j)!=0)    crc|=0x01;
				crc^=0x31;
			}
			else
			{
				crc=crc<<1;
				if (((*ptr)&j)!=0)    crc|=0x01;
			}
		}
		ptr++;
	}

	i=0;
	j=0;
	for (i=0x01; i<=0x80; (i=i<<1))
	{
		crcbit[j]=(crc&i)/(1<<j);
		j++;
	}
	crc=128*crcbit[0]+64*crcbit[1]+32*crcbit[2]+16*crcbit[3]+8*crcbit[4]+4*crcbit[5]+2*crcbit[6]+1*crcbit[7];

	return(crc);
}


BOOL  ds2411_validate( unsigned char *serialnumber)
{
	DD_RESULT   error = DD_OK;
	unsigned char familycode=0;
	unsigned char serialcode[6]={0,0,0,0,0,0};
	unsigned char  crccode=0;
	unsigned char crcbuff[8]={0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char *ptr;
	int i =0;
	UI16 count = 0;

	for (i=0; i<8; i++)
		serialnumber[i]=0;

	error = ds2411_pio_init();
	if (error != DD_OK)
	{
		return FALSE;
	}

	while(count<1000)
		{
		delay1us();//task_delay(ST_GetClocksPerSecond()/1000);
		count++;
		}
	
	#if 1
	error = ds2411_reset();
	#else	
	if (!ds2411_reset())
	{
		ds2411_pio_close();
		return FALSE;
	}
	#endif
	
	error = ds2411_pio_init();
	
	ds2411_writebyte(0x33);

	familycode=ds2411_readbyte();
	if ((familycode!=0x01) && (familycode!=0x81))
	{
		//ds2411_pio_close();
		return FALSE;
	}

	for (i=0; i<6; i++)
	{
		serialcode[i]=ds2411_readbyte();
	}

	crccode=ds2411_readbyte();

	crcbuff[0]=familycode;
	crcbuff[1]=serialcode[0];
	crcbuff[2]=serialcode[1];
	crcbuff[3]=serialcode[2];
	crcbuff[4]=serialcode[3];
	crcbuff[5]=serialcode[4];
	crcbuff[6]=serialcode[5];
	ptr=crcbuff;
	if (crccode!=ds2411_crc8(ptr,8))
	{
		//ds2411_pio_close();
		return FALSE;
	}

	serialnumber[0]=familycode;
	serialnumber[1]=serialcode[0];
	serialnumber[2]=serialcode[1];
	serialnumber[3]=serialcode[2];
	serialnumber[4]=serialcode[3];
	serialnumber[5]=serialcode[4];
	serialnumber[6]=serialcode[5];
	serialnumber[7]=crccode;

	return TRUE;
}

